program = 'w5blokusprocess'
"""checker for above named program.

Note: the author emails are not validated against the actual student list.
"""
TIMEALLOWED = 3

import subprocess
import random
import logging
import unittest
import os



# CODE FOR THIS CHECKER 

BADCHAR = 1
BADSIZE = 2
NOTILE = 3

simple_test_input=""".....
.***.
.....
.....
.....

.....
.....
.....
...**
.....

"""

four_input="""....
.***
....
....

....
**..
....
....

"""

three_input="""***
***
***

.**
.**
.**

"""

two_input="""**
..

**
.*

"""

seven_input=""".......
.......
.....*.
.......
.......
.......
.......

.......
.......
.......
.......
.......
.......
......*

*******
*******
*******
*******
.......
.......
*******

.......
.......
....*..
...**..
....*..
....*..
.****..

"""

empty_input='...**\n.....\n.....\n.....\n.....\n\n.....\n.....\n.....\n.....\n.....\n\n*....\n.....\n.....\n.....\n.....\n\n'

bad_size_inputs=[
 '...**\n.....\n.....\n.....\n.....\n\n.....\n.....\n.....\n...*.\n.....\n\n*....\n.....\n.....\n.....\n...\n\n',
 '...**\n.....\n.....\n.....\n.....\n.....\n.....\n.....\n...**\n.....\n\n*....\n.....\n.....\n.....\n.....\n\n',
 '...**\n.....\n.......\n.....\n.....\n\n.....\n.....\n....*\n*....\n.....\n\n*....\n.....\n.....\n.....\n.....\n\n',
 '...**\n.....\n.....\n.....\n.....\n\n.....\n.....\n..**.\n.....\n.....\n\n*....\n.....\n.....\n']


bad_char_inputs=[
('5','.....\n.***.\n.....\n.....\n.....\n\n.....\n.....\n..?..\n...**\n.....\n\n'),
('5','.....\n.***.\n.....\n.....\n.....\n\n.....\n.....\n.. ..\n...**\n.....\n\n'),
('5','.....\n.*@*.\n.....\n.....\n.....\n\n.....\n.....\n.....\n...**\n.....\n\n'),
('3','***\n***\n***\n\n.**\n.**\n.q*\n\n'),
('3','***\n***\n*x*\n\n.**\n.**\n.q*\n\n')]

class BlokusTestCase(unittest.TestCase):
    def setUp(self):
        self.Parms={}
        self.Parms['timeout']=TIMEALLOWED
        self.Parms['stdout']=subprocess.PIPE

    def test_empty_tile(self):
        """check invalid tilesets: empty textbox in a tileset"""
        self.Parms['input']=empty_input.encode()
        R = subprocess.run(['./'+program],**self.Parms)
        if R.returncode != NOTILE:
            self.fail('Empty tilesets should generate a return code of {}. You returned {}'.format(NOTILE,R.returncode))
        if R.stdout.decode():
            self.fail('Output should be empty when an empty tileset is encountered.')

    def test_badsize_tilesets(self):
        """check invalid tilesets: bad size cases"""
        for badin in bad_size_inputs:
          with self.subTest(CASE=badin):
            self.Parms['input']=badin.encode()
            R = subprocess.run(['./'+program],**self.Parms)

            if R.returncode != BADSIZE:
                self.fail('The following tileset is invalid:\n{} and should result in a return code of {}'.format(badin,BADSIZE))
            if R.stdout.decode():
                self.fail('Output should be empty when an invalid tileset is encountered.')


    def test_badchar_tilesets(self):
        """check invalid tilesets: bad character cases"""
        for probsize,badin in bad_char_inputs:
          with self.subTest(CASE=badin):
            self.Parms['input']=badin.encode()
            R = subprocess.run(['./'+program,probsize],**self.Parms)

            if R.returncode != BADCHAR:
                self.fail('The following tileset is invalid:\n{} and should result in a return code of {}.\n Your return code was {}'.format(badin,BADCHAR,R.returncode))
            if R.stdout.decode():
                self.fail('Output should be empty when an invalid tileset is encountered.')

    def test_zero_args(self):
        """check that cin and cout are used correctly, and that 5x5 grids are expected"""
        R = subprocess.run(['./'+program],input=simple_test_input.encode(), **self.Parms)
        self.assertEqual(R.returncode,0)
        self.assertEqual(simple_test_input,R.stdout.decode())

    def test_one_args(self):
        """check that cin and cout are used correctly, and that 4x4 grids are expected"""
        R = subprocess.run(['./'+program,"4"],input=four_input.encode(),**self.Parms)
        self.assertEqual(R.returncode,0)
        self.assertEqual(four_input,R.stdout.decode())

    def test_seven(self):
        """check that 7x7 grids are expected"""
        R = subprocess.run(['./'+program,"7"],input=seven_input.encode(),**self.Parms)
        self.assertEqual(R.returncode,0)
        self.assertEqual(seven_input,R.stdout.decode())

    def test_two_args(self):
        """check that an input file can be specified and that 3x3 grids are expected"""
        infilename = "myin{}.txt".format(random.randint(1000,100000))
        with open(infilename,'w') as f:
            f.write(three_input)

        R = subprocess.run(['./'+program,"3",infilename],**self.Parms)
        os.remove(infilename)
        self.assertEqual(R.returncode,0)
        self.assertEqual(three_input,R.stdout.decode())


    def test_three_args(self):
        """check that input and output files can be specified and that 2x2 grids are expected"""
        infilename = "myin{}.txt".format(random.randint(1000,100000))
        outfilename = "myout{}.txt".format(random.randint(1000,100000))

        with open(infilename,'w') as f:
            f.write(two_input)

        R = subprocess.run(['./'+program,"2",infilename,outfilename],**self.Parms)
        os.remove(infilename)
        if R.stdout.decode():
            self.fail('Output to cout detected when output file specified.')

        two_output = open(outfilename).read()
        os.remove(outfilename)
        self.assertEqual(R.returncode,0)
        self.assertEqual(two_input,two_output)

    def test_more_args(self):
        """check that extra arguments are ignored and that 2x2 grids are expected"""
        infilename = "myin{}.txt".format(random.randint(1000,100000))
        outfilename = "myout{}.txt".format(random.randint(1000,100000))

        with open(infilename,'w') as f:
            f.write(two_input)

        R = subprocess.run(['./'+program,"2",infilename,outfilename,"test","extra","arguments"],**self.Parms)
        os.remove(infilename)
        if R.stdout.decode():
            self.fail('Output to cout detected when output file specified.')

        two_output = open(outfilename).read()
        os.remove(outfilename)
        self.assertEqual(R.returncode,0)
        self.assertEqual(two_input,two_output)




# GENERIC CHECKER CODE


stylemsgs=[
" Your code is super styling.",
" Wow, what great style you have!",
" Nice style!",
" You have mastered space!"]

program_name = program+'.cpp'

orig_program = program_name

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
    """return any errors as a list of strings"""
    errors = []
    passed = []
    loader = unittest.loader.TestLoader()
    tests = loader.loadTestsFromTestCase(BlokusTestCase)
    for t in sorted(tests,key=lambda x: x.shortDescription()):
        x=t.run()
        if x.wasSuccessful():
            passed.append('Passed: {}\n'.format(t.shortDescription()))
        else:
            err = 'Failed: {}\n'.format(t.shortDescription())
            for test,res in x.failures+x.errors:
                if 'AssertionError:' in res:
                    _,msg=res.split('AssertionError:')
                else:
                    msg = res
                err += msg
            errors.append(err)

    return errors,passed

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

    P_astyle = subprocess.run(['astyle','--formatted','--dry-run',
           '--style=google','--indent=spaces=2',program_name],
           stdout=subprocess.PIPE,stderr=subprocess.PIPE)
    if P_astyle.returncode:
        s += 'astyle returned an error: {}'.format(P_astyle.stderr.decode())

    if P_astyle.stdout.decode().startswith('Formatted'):
        s +='astyle made changes, please use "astyle --style=google --indent=spaces=2" before submitting\n'
        astyle_error = True
    else:
        s += "astyle ok\n"
        astyle_error = False

    P_lint = subprocess.run(['cpplint','--filter=-readability/alt_tokens',program_name], stderr=subprocess.PIPE)
    if P_lint.returncode:
        s += 'cpplint found problems, as follows:\n'
        s += P_lint.stderr.decode()+"\n"
        cpplint_error = True
    else:
        s += 'cpplint ok\n'
        cpplint_error = False
    
    if not cpplint_error and not astyle_error:
       s += random.choice(stylemsgs)

    restricted_includes = False
    
    s += '\n---- compile check ----\n'
    C = subprocess.run(['g++','--std=gnu++14',program_name, '-o', program], stderr=subprocess.PIPE)
    if C.returncode:
        s += 'g++ found problems, as follows:'
        s += C.stderr.decode()
        return 'No compile',s
    else:
        s += 'pass\n'

    s += '\n---- program check ----\n'
    errors,passed = check_program()
    for p in passed:
        s += p

    if errors:
        s += '-----------------errors found--------------\n'
        for e in errors:
            s += e + "\n-------\n"
    else:
        s += 'pass\n'

    if errors:
        return 'Errors',s
    elif P_lint.returncode or astyle_error:
        return 'Lint found and/or style error',s
    elif restricted_includes:
        return "Restricted includes",s


    return 'Pass',s


if __name__ == '__main__':
    summary,results = main()
    print(results)
    print('\nSummary:', summary)
