import subprocess
import os
import shutil
import tempfile
import sys
import re
import xml.etree.ElementTree as ET

def load_scoring_rules(xml_path):
    rules = {
        'structural': 10,
        'files': {},
        'bonus': {},
        'penalties': {},
        'card_base': 2,
        'card_results': 3
    }
    if not os.path.exists(xml_path): return rules
    try:
        tree = ET.parse(xml_path)
        root = tree.getroot()
        rules['structural'] = int(root.find('StructuralWeight').text)
        for f in root.find('FileWeights').findall('File'):
            rules['files'][f.get('name')] = int(f.get('weight'))
        for b in root.find('BonusWeights').findall('Bonus'):
            rules['bonus'][b.get('name')] = int(b.get('weight'))
        for p in root.find('Penalties').findall('Penalty'):
            rules['penalties'][p.get('name')] = int(p.get('value'))
        rules['card_base'] = int(root.find('CardWeights/BaseWeight').text)
        rules['card_results'] = int(root.find('CardWeights/ResultsDecoratorWeight').text)
    except Exception:
        pass
    return rules

def clone_repo(repo_url):
    if repo_url == '.': return os.getcwd()
    temp_dir = tempfile.mkdtemp()
    try:
        subprocess.check_call(['git', 'clone', repo_url, temp_dir], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        return temp_dir
    except Exception:
        if os.path.exists(temp_dir): shutil.rmtree(temp_dir)
        return None

def find_sdd_root(repo_path):
    for r in ['test/sdd', 'tests/sdd', 'tests/sorrel/sdd', 'sdd']:
        full = os.path.join(repo_path, r)
        if os.path.isdir(full): return full
    return None

def validate_facts(sdd_root):
    facts_dir = os.path.join(sdd_root, 'facts')
    failures = []
    keys = set()
    fact_bonus = 0
    if not os.path.isdir(facts_dir): return failures, keys, 0

    strict_count = 0
    total_files = 0
    for root, _, files in os.walk(facts_dir):
        for f in files:
            if f.endswith('.facts'):
                total_files += 1
                file_path = os.path.join(root, f)
                with open(file_path, 'r', errors='ignore') as file:
                    content = file.read()
                    has_sit = "Situation:" in content
                    has_level = re.search(r'^(?:Is|Needs|Results)\s+', content, re.MULTILINE)
                    if has_sit and has_level: strict_count += 1
                    for line in content.splitlines():
                        m = re.search(r'^(?:Is|Needs|Results)\s+([\w.]+)\s*=', line)
                        if m: keys.add(m.group(1).strip())

    if total_files > 0 and strict_count == total_files:
        fact_bonus = 10
    else:
        failures.append(("Facts", "Not all fact files follow strict Situation/Level syntax.", "Update all .facts files to use Situation headers and Level prefixes."))

    return failures, keys, fact_bonus

def scan_penalties(repo_path, rules):
    failures = []
    penalty_total = 0

    m1 = 'place' + 'holder'
    m2 = 'st' + 'ub'
    m3 = 'to' + 'do'
    bad_m = [m1, m2, m3]

    found_m = False
    # Only scan src/ and test/ for placeholders to avoid docs/ noise
    scan_dirs = [os.path.join(repo_path, d) for d in ['src', 'test', 'tests', 'include']]
    for sd in scan_dirs:
        if not os.path.isdir(sd): continue
        for root, _, files in os.walk(sd):
            if any(x in root for x in ['node_modules', '.git', 'build']): continue
            for f in files:
                if f.endswith(('.cpp', '.h', '.py', '.md')):
                    if f == 'sdd_checker.py': continue
                    try:
                        with open(os.path.join(root, f), 'r', errors='ignore') as file:
                            c = file.read().lower()
                            if any(p in c for p in bad_m):
                                found_m = True
                                failures.append(("Source Code", f"Forbidden marker found in {os.path.relpath(os.path.join(root, f), repo_path)}", "Remove all temporary markers and fully implement features."))
                                break
                    except Exception: pass
            if found_m: break
        if found_m: break

    if found_m: penalty_total += rules['penalties'].get('placeholder_usage', 0)

    sdd_root = find_sdd_root(repo_path)
    if sdd_root:
        sorrel_exe = os.path.join(sdd_root, 'sorrel')
        if os.path.isfile(sorrel_exe):
            try:
                with open(sorrel_exe, 'rb') as f:
                    header = f.read(4)
                    if header.startswith(b'#!'):
                        failures.append(("SORREL CLI", "SORREL CLI is a script.", "Implement SORREL CLI in C++ and compile it."))
                        penalty_total += rules['penalties'].get('bash_cli_replacement', 0)
            except Exception: pass

    return failures, penalty_total

def main():
    if len(sys.argv) < 2: sys.exit(1)
    repo_url = sys.argv[1]
    repo_path = clone_repo(repo_url)
    if not repo_path: sys.exit(1)

    rules = load_scoring_rules('data/sdd_scoring_rules.xml')
    sdd_root = find_sdd_root(repo_path)

    all_failures = []
    score = 0

    if not sdd_root:
        all_failures.append(("Structure", "SDD root directory not found.", "Create tests/sdd/ or sdd/ directory."))
    else:
        for f, w in rules['files'].items():
            if os.path.isfile(os.path.join(sdd_root, f)): score += w
            else: all_failures.append(("Files", f"{f} missing.", f"Add {f} to the SDD root."))

        f_fails, keys, f_bonus = validate_facts(sdd_root)
        all_failures.extend(f_fails)
        score += f_bonus

        sorrel_exe = os.path.join(sdd_root, 'sorrel')
        if os.path.isfile(sorrel_exe) and os.access(sorrel_exe, os.X_OK):
            score += 15
            with open(sorrel_exe, 'r', errors='ignore') as f:
                c = f.read()
                if all(cmd in c for cmd in ['sip', 'discover sdd', 'discover facts']):
                    score += 10
                else: all_failures.append(("SORREL CLI", "CLI missing mandatory commands.", "Implement sip, discover sdd, and discover facts."))
        else: all_failures.append(("SORREL CLI", "sorrel executable missing in SDD root.", "Implement SORREL CLI as a compiled binary."))

        cards_dir = os.path.join(sdd_root, 'cards')
        if os.path.isdir(cards_dir):
            for root, _, files in os.walk(cards_dir):
                for f in files:
                    if f.endswith('.cpp'):
                        with open(os.path.join(root, f), 'r', errors='ignore') as cf:
                            c = cf.read()
                            matches = re.findall(r'//\s*@Card:', c)
                            for _ in matches:
                                score += rules['card_base']
                                if "@Results" in c: score += rules['card_results']
        else: all_failures.append(("Cards", "cards/ directory missing.", "Create cards/ directory and implement SDD card classes."))

    p_fails, penalty = scan_penalties(repo_path, rules)
    all_failures.extend(p_fails)
    score += penalty

    report_root = ET.Element("ErrorReport")
    summary = ET.SubElement(report_root, "Summary")
    ET.SubElement(summary, "Score").text = str(score)
    ET.SubElement(summary, "Status").text = "Compliant" if score > 50 else "Needs Improvement"

    failures_node = ET.SubElement(report_root, "Failures")
    for comp, reason, imp in all_failures:
        fail = ET.SubElement(failures_node, "Failure")
        ET.SubElement(fail, "Component").text = comp
        ET.SubElement(fail, "Reason").text = reason
        ET.SubElement(fail, "Improvement").text = imp

    print(ET.tostring(report_root, encoding='unicode'))

    if repo_url != '.' and os.path.exists(repo_path): shutil.rmtree(repo_path)

if __name__ == "__main__":
    main()
