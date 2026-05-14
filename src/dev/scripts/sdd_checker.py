import subprocess
import os
import shutil
import tempfile
import sys
import re

def clone_repo(repo_url):
    if repo_url == '.':
        return os.getcwd()

    temp_dir = tempfile.mkdtemp()
    try:
        subprocess.check_call(['git', 'clone', repo_url, temp_dir], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        return temp_dir
    except subprocess.CalledProcessError as e:
        print(f"Error cloning repository: {e}")
        shutil.rmtree(temp_dir)
        return None

def find_sdd_root(repo_path):
    possible_roots = [
        'test/sdd',
        'tests/sdd',
        'tests/sorrel/sdd',
        'sdd'
    ]
    if os.path.isfile(os.path.join(repo_path, 'restrictions.md')) and \
       os.path.isdir(os.path.join(repo_path, 'cards')):
        return repo_path

    for root in possible_roots:
        full_path = os.path.join(repo_path, root)
        if os.path.isdir(full_path):
            return full_path
    return None

def get_fact_keys(sdd_root):
    facts_dir = os.path.join(sdd_root, 'facts')
    keys = set()
    if not os.path.isdir(facts_dir):
        return keys

    for root, _, files in os.walk(facts_dir):
        for f in files:
            if f.endswith('.facts'):
                with open(os.path.join(root, f), 'r', errors='ignore') as file:
                    for line in file:
                        match = re.search(r'(?:Is|Needs|Results)\s+([\w.]+)\s*=', line)
                        if match:
                            keys.add(match.group(1).strip())
                        elif '=' in line and 'Situation:' not in line and not line.strip().startswith('#'):
                            key = line.split('=')[0].strip()
                            if key: keys.add(key)
    return keys

def validate_cards(sdd_root, defined_keys):
    cards_dir = os.path.join(sdd_root, 'cards')
    if not os.path.isdir(cards_dir):
        return [" [FAIL] cards/ directory not found."], 0, 0, set()

    report = []
    card_count = 0
    valid_card_count = 0
    all_card_names = set()
    used_keys = set()

    for root, _, files in os.walk(cards_dir):
        for f in files:
            if f.endswith('.cpp'):
                file_path = os.path.join(root, f)
                with open(file_path, 'r', errors='ignore') as card_file:
                    content = card_file.read()
                    sections = re.split(r'(?=//\s*@Card:)', content)
                    for section in sections:
                        match = re.search(r'//\s*@Card:\s*([\w\s]+)', section)
                        if match:
                            card_name = match.group(1).strip()
                            card_count += 1

                            if card_name in all_card_names:
                                report.append(f" [FAIL] Duplicate card name '{card_name}' found.")
                            else:
                                all_card_names.add(card_name)

                            has_results = re.search(r'//\s*@Results\s+', section)
                            if has_results:
                                valid_card_count += 1
                                report.append(f" [PASS] Card '{card_name}' in {f} has @Results.")
                            else:
                                report.append(f" [WARN] Card '{card_name}' in {f} missing @Results.")

                            # Extract used keys
                            found_keys = re.findall(r'//\s*@(?:Is|Needs|Results)\s+([\w.]+)\s*==', section)
                            for key in found_keys:
                                used_keys.add(key)
                                if defined_keys and key not in defined_keys:
                                    report.append(f" [WARN] Card '{card_name}' uses undefined fact key: {key}")

    return report, valid_card_count, card_count, used_keys

def validate_restrictions(sdd_root):
    res_path = os.path.join(sdd_root, 'restrictions.md')
    if not os.path.isfile(res_path):
        return [f"[FAIL] restrictions.md missing."]

    report = []
    with open(res_path, 'r', errors='ignore') as f:
        content = f.read()
        sections = [
            "Pattern Restrictions",
            "Tool Restrictions",
            "Architectural Restrictions",
            "Validation Restrictions"
        ]
        for sec in sections:
            if sec in content:
                report.append(f" [PASS] Restriction section found: {sec}")
            else:
                report.append(f" [WARN] Restriction section missing: {sec}")
    return report

def main():
    if len(sys.argv) < 2:
        print("Usage: python sdd_checker.py <repo_url>")
        sys.exit(1)

    repo_url = sys.argv[1]
    repo_path = clone_repo(repo_url)
    if not repo_path: sys.exit(1)

    sdd_root = find_sdd_root(repo_path)
    if not sdd_root:
        print("Error: Could not find SDD root directory.")
        if repo_url != '.' and os.path.exists(repo_path): shutil.rmtree(repo_path)
        sys.exit(1)

    print(f"--- SORREL Adherence Report ---")
    print(f"Target: {repo_url}")
    print("-" * 30)

    # 1. Structural Check
    required = ['sorrel_checkins.md', 'sorrel_checkouts.md', 'restrictions.md']
    struct_score = 0
    for f in required:
        if os.path.isfile(os.path.join(sdd_root, f)):
            print(f"[PASS] File found: {f}")
            struct_score += 1
        else:
            print(f"[FAIL] File missing: {f}")

    # Check for card_runner.cpp
    has_runner = os.path.isfile(os.path.join(sdd_root, 'card_runner.cpp'))
    if has_runner:
        print("[PASS] card_runner.cpp found.")
        struct_score += 1
    else:
        print("[WARN] card_runner.cpp missing (required for full Sorrel autonomy).")

    # 2. Restrictions Check
    res_report = validate_restrictions(sdd_root)
    for line in res_report: print(line)

    # 3. Facts and Cards Check
    defined_keys = get_fact_keys(sdd_root)
    card_report, card_valid, card_total, used_keys = validate_cards(sdd_root, defined_keys)
    for line in card_report: print(line)

    print("-" * 30)
    total_possible = len(required) + 1 + card_total + len(used_keys)
    actual_score = struct_score + card_valid
    # Add score for validated keys
    for k in used_keys:
        if k in defined_keys: actual_score += 1

    if total_possible > 0:
        print(f"Overall Adherence Score: {actual_score}/{total_possible} ({ (actual_score/total_possible)*100:.1f}%)")

    if repo_url != '.' and os.path.exists(repo_path): shutil.rmtree(repo_path)

if __name__ == "__main__":
    main()
