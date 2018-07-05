"""checker for w1poem

Note: the author emails are not validated against the actual student list.
"""

import subprocess as sub

program = 'w1poem'
program_name = program+'.cpp'


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
        R = sub.run(['./'+program], stdout=sub.PIPE)
        lines = R.stdout.decode().splitlines()
        if len(lines) != 3:
            return ['Number of poetry lines incorrect: {}'.format(len(lines))]

        if not (len(lines[0]) < len(lines[1]) < len(lines[2])):
            return ['Your poem is not the correct shape']

    except Exception as e:
        print(e)
        return ['Program caused checker to crash.']


def main():
    s = 'Checking w1poem.cpp for EC327 submission.\n'
    try:
        the_program = open(program_name).read()
    except:
        s += 'The program {} does not exist here.\n'.format(program_name)
        return 'No file',s

    s += '\n---- author check ----\n'
    authors = get_authors(the_program)
    s += 'The authors are: {}\n'.format(" ".join(authors))
    if len(authors) > 1:
        s += 'For this assignment, only one author is permitted.\n'

    s += '\n---- style check ----\n'
    T = sub.run(['cpplint', program_name], stderr=sub.PIPE)
    if T.returncode:
        s += 'cpplint found problems, as follows:\n'
        s += T.stderr.decode()+"\n"
    else:
        s += 'pass\n'

    s += '\n---- compile check ----\n'
    C = sub.run(['g++', program_name, '-o', program], stderr=sub.PIPE)
    if C.returncode:
        s += 'g++ found problems, as follows:'
        s += C.stderr.decode()
        return 'No compile',s
    else:
        s += 'pass\n'

    s += '\n---- program check ----\n'

    errors = check_program()
    if errors:
        s += 'errors found:'
        for e in errors:
            s += e + "\n"
    else:
        s += 'pass\n'

    if errors:
        return 'Errors',s
    elif T.returncode:
        return 'Lint found',s
    elif len(authors) > 1:
        return "Too many authors",s

    return 'Pass',s


if __name__ == '__main__':
    summary,results = main()
    print(results)
    print('\nSummary:', summary)
