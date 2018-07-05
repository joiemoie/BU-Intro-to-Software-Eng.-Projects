program = 'w2happy'
"""checker for above named program.

Note: the author emails are not validated against the actual student list.
"""
TIMEALLOWED = 2

import subprocess
import hashlib
import re

program_name = program+'.cpp'

orig_program = program_name

def get_includes(file_contents):
    IncludeList=[]
    for line in file_contents.lower().splitlines():
        if "include" in line:
            m = re.match("\s?#\s?include\s?(<\w+>).*",line)
            if m and m.groups():
                IncludeList.append(m.groups()[0])
    return IncludeList

def get_authors(file_contents):
    Authors = []
    findstr = "// copyright"

    for line in file_contents.lower().splitlines():
        if line.startswith(findstr):
            try:
                _, email = line.rsplit(" ", 1)
                if email.endswith('@bu.edu'):
                    Authors.append(email)
            except:
                pass
    return Authors


def check_program():
    try:
        R = subprocess.run(['./'+program], stdout=subprocess.PIPE,timeout=TIMEALLOWED)
        txt = ",".join(R.stdout.decode().strip().split())

        h = hashlib.sha256()
        h.update(txt.encode())
        summary = h.hexdigest()     
        if summary != '15d93c75983d0d6edb2a2ebaae2b2f2508ed481ae761016095ee84d3a8e044a3':
            return ['Output is not perfect. Keep trying.']
    except subprocess.TimeoutExpired:
        return ['Program did not complete in the time allocated ({} seconds).'.format(TIMEALLOWED)]
    except Exception as e:
        print(type(e),e)
        return ['Program caused checker to crash.']


def main():
    s = 'Checking {} for EC327 submission.\n'.format(orig_program)
    try:
        the_program = open(program_name).read()
    except:
        s += 'The program {} does not exist here.\n'.format(orig_program)
        return 'No file',s

    s += '\n---- author check ----\n'
    authors = get_authors(the_program)
    s += 'The authors are: {}\n'.format(" ".join(authors))

    s += '\n---- style check ----\n'
    T = subprocess.run(['cpplint','--filter=-readability/alt_tokens',program_name], stderr=subprocess.PIPE)
    if T.returncode:
        s += 'cpplint found problems, as follows:\n'
        s += T.stderr.decode()+"\n"
    else:
        s += 'pass\n'

    s += '\n---- restrictions check ----\n'

    include_list = get_includes(the_program)
    if include_list != ['<iostream>']:
        s += 'include files are restricted to <iostream>\n'
        s += 'you have included: {}\n'.format(" ".join(include_list))
        restricted_includes = True
    else:
        restricted_includes = False
        s += 'pass\n'

    s += '\n---- compile check ----\n'
    C = subprocess.run(['g++', program_name, '-o', program], stderr=subprocess.PIPE)
    if C.returncode:
        s += 'g++ found problems, as follows:'
        s += C.stderr.decode()
        return 'No compile',s
    else:
        s += 'pass\n'

    s += '\n---- program check ----\n'

    errors = check_program()
    if errors:
        s += 'errors found:\n'
        for e in errors:
            s += " " + e + "\n"
    else:
        s += 'pass\n'

    if errors:
        return 'Errors',s
    elif T.returncode:
        return 'Lint found',s
    elif restricted_includes:
        return "Restricted includes",s


    return 'Pass',s


if __name__ == '__main__':
    summary,results = main()
    print(results)
    print('\nSummary:', summary)
