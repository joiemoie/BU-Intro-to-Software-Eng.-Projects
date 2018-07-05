program = 'w3strings'
"""checker for above named program.

Note: the author emails are not validated against the actual student list.
"""
TIMEALLOWED = 2

import subprocess
import hashlib
import re
import random

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

real_main=r"""// leave this line and everything below as is
int main() {
  cout << std::boolalpha;

  if (not is_word("test")) cout << "we1\n";
  if (not is_word("Test")) cout << "we2\n";
  if (not is_word("TEST")) cout << "we3\n";
  if (not is_word("thisisaword")) cout << "we4\n";

  if (is_word("123")) cout << "we5\n";
  if (is_word("")) cout << "we6\n";
  if (is_word("abc123abc")) cout << "we7\n";
  if (is_word("tEst")) cout << "we8\n";
  if (is_word("tEst")) cout << "we9\n";
  if (is_word("TESTer")) cout << "we10\n";

  bool error;

  if (not is_palindrome("12321", error) or error) cout << "pe1\n";
  if (not is_palindrome("9009", error) or error) cout  << "pe2\n";
  if (not is_palindrome("9", error) or error)  cout << "pe3\n";
  if (not is_palindrome("123456777654321", error) or error) cout << "pe4\n";

  if (is_palindrome("abcba", error) or not error) cout << "pe5\n";
  if (is_palindrome("12321 a", error) or not error) cout << "pe6\n";
  if (is_palindrome("0012100", error) or not error) cout << "pe7\n";

  if (is_palindrome("123", error) or error) cout << "pe8\n";
  if (is_palindrome("123211", error) or error) cout << "pe9\n";

  cout << "Test Other Words. Ctrl-D to exit.\n";

  string s;
  bool pal;
  while (cin >> s) {
    pal = is_palindrome(s, error);
    cout << s << " " << pal << " " << error << " " << is_word(s) << '\n';
  }

  return 0;
}
"""

isword_cases={'false':['AbcdZ','`aaa','AA@A','ZZZ[','zzz{z','ninE','tEn','12321','aI',
'OPERAtor','uP',"abcdefghi1"],'true':['eleven','braces','Six','seven','EIGHT','a',
'I','Z','z','On','NO']}

ispalindrome_cases = {
('true','false'):  ['123321','99999','9876543223456789','9','1','44'], # good palindromes
('false','true'): ['abc','-123','4.4','012321','123b321'], # bad numbers
('false','false'): ['123','3433','3343','9999999999999999991','1222222222'] # good numbers, not palindromes
}
# output is 
# word is_pal error is_word

def check_isword():
    Vals=[]
    Ans={}
    Errors=[]
    for cond in isword_cases:
        for word in isword_cases[cond]:
            Vals.append(word)
            Ans[word]=cond
    random.shuffle(Vals)
    input_str = "\n".join(Vals)
    try:
        R = subprocess.run(['./'+program], input=input_str.encode(),stdout=subprocess.PIPE,timeout=TIMEALLOWED)
        std_tests,new_tests=R.stdout.decode().split('Test Other Words. Ctrl-D to exit.\n')

        if std_tests:
           for res in std_tests.splitlines():
             if res.startswith('w'): Errors.append(res)

        for result in new_tests.splitlines():
           word,_,_,is_word_answer = result.split()
           if Ans[word] != is_word_answer:
             Errors.append('Wrong result for is_word("{}"), should be {}'.format(word,Ans[word]))
    except subprocess.TimeoutExpired:
        return ['Program did not complete in the time allocated ({} seconds).'.format(TIMEALLOWED)]
    except Exception as e:
        print(type(e),e)
        return ['Program caused checker to crash.']

    return Errors
  

def check_ispalindrome():
    Vals=[]
    Ans={}
    Errors=[]
    for conds in ispalindrome_cases:
        for word in ispalindrome_cases[conds]:
            Vals.append(word)
            Ans[word] = conds
    random.shuffle(Vals)
    input_str = "\n".join(Vals)
    try:
        R = subprocess.run(['./'+program], input=input_str.encode(),stdout=subprocess.PIPE,timeout=TIMEALLOWED)
        std_tests,new_tests=R.stdout.decode().split('Test Other Words. Ctrl-D to exit.\n')

        if std_tests:
           for res in std_tests.splitlines():
              if res.startswith('p'): Errors.append(res)

        for result in new_tests.splitlines():
           word,ispal,error,_ = result.split()
           if Ans[word] != (ispal,error):
             Errors.append('Wrong result for is_palindrome("{}",error), should be {} and error should be {}'.format(word,Ans[word][0],Ans[word][1]))
 
    except subprocess.TimeoutExpired:
        return ['Program did not complete in the time allocated ({} seconds).'.format(TIMEALLOWED)]
    except Exception as e:
        print(type(e),e)
        return ['Program caused checker to crash.']
    return Errors


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
    T = subprocess.run(['cpplint','--filter=-readability/alt_tokens,-runtime/references',program_name], stderr=subprocess.PIPE)
    if T.returncode:
        s += 'cpplint found problems, as follows:\n'
        s += T.stderr.decode()+"\n"
    else:
        s += 'pass\n'
    
    s += '\n---- main check (using standard main() ) ----\n'

    if the_program.endswith(real_main):
        s += 'pass\n'
    else:
        first_part,second_part=the_program.split('// leave this line and everything below as is\n')
        for i,(x,y) in enumerate(zip(second_part.splitlines(),real_main.splitlines()[1:])):
           if x != y:
             print('mismatch at',i)
             print(x)
             print(y)

        s += 'Bad main\n'

    restricted_includes = False
    
    s += '\n---- compile check ----\n'
    C = subprocess.run(['g++','--std=gnu++14',program_name, '-o', program], stderr=subprocess.PIPE)
    if C.returncode:
        s += 'g++ found problems, as follows:'
        s += C.stderr.decode()
        return 'No compile',s
    else:
        s += 'pass\n'

    s += '\n---- isword check ----\n'

    worderrors = check_isword()
    if worderrors:
        s += 'errors found:\n'
        for e in worderrors:
            s += " " + e + "\n"
    else:
        s += 'pass\n'

    s += '\n---- ispalindrome check ----\n'

    palerrors = check_ispalindrome()
    if palerrors:
        s += 'errors found:\n'
        for e in palerrors:
            s += " " + e + "\n"
    else:
        s += 'pass\n'

    if worderrors or palerrors:
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
