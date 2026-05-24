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
        'card_results': 3,
        'green_syntax': 5,
        'numeric_evidence': 5
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
        # Precision Power extensions
        if root.find('PrecisionPower'):
            rules['green_syntax'] = int(root.find('PrecisionPower/GreenSyntaxWeight').text)
            rules['numeric_evidence'] = int(root.find('PrecisionPower/NumericEvidenceWeight').text)
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
                    has_is = "Is " in content
                    if has_sit and has_is: strict_count += 1
                    for line in content.splitlines():
                        m = re.search(r'^(?:Is|Needs|Results)\s+([\w.]+)\s*=', line)
                        if m:
                            val = line.split('=')[1].strip()
                            if val in ['0', '1', 'true', 'false']:
                                failures.append(("Facts", f"Lazy boolean/binary value '{val}' in {f}", "Use descriptive numeric metrics."))
                            keys.add(m.group(1).strip())

    if total_files > 0 and strict_count == total_files:
        fact_bonus = 10
    else:
        failures.append(("Facts", "Not all fact files follow strict Situation/Is syntax.", "Update all .facts files to use Situation headers and Level prefixes."))

    return failures, keys, fact_bonus

def validate_ledgers(sdd_root):
    failures = []
    bonus = 0
    checkins = os.path.join(sdd_root, 'sorrel_checkins.md')
    checkouts = os.path.join(sdd_root, 'sorrel_checkouts.md')

    if os.path.exists(checkins) and os.path.exists(checkouts):
        bonus += 10
        with open(checkouts, 'r', errors='ignore') as f:
            content = f.read()
            # Check for numeric evidence in checkouts, penalize 0/1
            matches = re.findall(r'(\w+)\s*=\s*(\d+)', content)
            if matches:
                lazy_count = sum(1 for k, v in matches if v in ['0', '1'])
                if lazy_count > 0:
                    failures.append(("Ledgers", f"Found {lazy_count} lazy binary observations (0/1).", "Use high-fidelity numeric metrics."))
                else:
                    bonus += 10
            else:
                failures.append(("Ledgers", "No numeric evidence found in sorrel_checkouts.md", "Include machine-parseable numeric observations."))
    else:
        failures.append(("Ledgers", "Dual-ledger files missing.", "Ensure sorrel_checkins.md and sorrel_checkouts.md exist."))

    return failures, bonus

def scan_penalties(repo_path, rules):
    failures = []
    penalty_total = 0

    bad_m = ['place' + 'holder', 'st' + 'ub', 'to' + 'do']

    found_m = False
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
                                failures.append(("Source Code", f"Forbidden marker found in {os.path.relpath(os.path.join(root, f), repo_path)}", "Remove all temporary markers."))
                                break
                    except Exception: pass
            if found_m: break
        if found_m: break

    if found_m: penalty_total += rules['penalties'].get('placeholder_usage', 0)

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
        all_failures.append(("Structure", "SDD root directory not found.", "Create tests/sdd/ directory."))
    else:
        for f, w in rules['files'].items():
            if os.path.isfile(os.path.join(sdd_root, f)): score += w
            else: all_failures.append(("Files", f"{f} missing.", f"Add {f} to the SDD root."))

        f_fails, keys, f_bonus = validate_facts(sdd_root)
        all_failures.extend(f_fails)
        score += f_bonus

        l_fails, l_bonus = validate_ledgers(sdd_root)
        all_failures.extend(l_fails)
        score += l_bonus

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
                                if "TOOLS" in c and "PARAMETERS" in c and "RESULTS" in c:
                                    score += rules['green_syntax']
                                else:
                                    all_failures.append(("Cards", f"Green Syntax missing in {f}", "Add TOOLS, PARAMETERS, and RESULTS blocks."))
                                # Precision Power check: Anti-Laziness (no 0/1)
                                res_matches = re.findall(r'//\s*@Results\s+\w+\s*==\s*(\d+)', c)
                                if res_matches:
                                    if all(v not in ['0', '1'] for v in res_matches):
                                        score += rules['numeric_evidence']
                                    else:
                                        all_failures.append(("Cards", f"Lazy binary result (0/1) in {f}", "Use descriptive numeric metrics."))
                                else:
                                    all_failures.append(("Cards", f"Numeric Evidence missing in {f}", "Ensure @Results decorator uses numeric comparisons."))
        else: all_failures.append(("Cards", "cards/ directory missing.", "Create cards/ directory."))

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
