program = 'w10collisions'
"""checker for above named program.

Note: the author emails are not validated against the actual student list.
"""
TIMEALLOWED = 2

SERVER = False

if SERVER:
    TIMEALLOWEDLONG = 10
else:
    TIMEALLOWEDLONG = 100

import subprocess
import random
import logging
import unittest
import difflib
import os
import re
import time


CPPLINT_OPTIONS=['-readability/alt_tokens']

ASTYLE_OPTIONS=['--style=google','--indent=spaces=2']


# CODE FOR THIS CHECKER 

FORBIDDEN_INCLUDES=['<fstream>']

SingleTestCases=['fastslow','random100','random10','random6','random1000']
PairedTestCases={'three':['threeway1','threeway2'],'updown':['updownchain2','updownchain'],'four':['fourway1','fourway2'] }

LongTestCases = ['random2k','random10k','random30k']

Coordfiles={}
Results={}

try:
    C = subprocess.run(['g++','-std=gnu++14','-O3','processor_check.cpp', '-o', 'processor_check'], stderr=subprocess.PIPE)
except:
    print('You must have processor_check.cpp to run this checker')
    quit()

st=time.time()
P = subprocess.run(['./processor_check'],stderr=subprocess.PIPE,stdout=subprocess.PIPE)
en=time.time()

your_speed = 1/(en-st)


def code_analysis(prog):
    T=subprocess.run(['g++','-std=gnu++14','-P','-x' ,'c++','-dD','-E','-fpreprocessed',prog],stdout=subprocess.PIPE)
    code = T.stdout.decode()

    lines = code.splitlines()
    words = code.split()
    return (len(lines),len(words),len(code))

def run_it(program,cases):
    times = {}
    for tc in cases:
        st=time.time()
        try:
            P = subprocess.run(['./'+program],input=Coordfiles[tc],stdout=subprocess.PIPE,timeout=TIMEALLOWEDLONG)
        except:
            pass
        en=time.time()

        times[tc]=your_speed * (en-st)
    return times


def processanswers(s):
    try:
        vehicle_report,rest=s.split('\ncollision report\n')
    except:
        return (False,'Could not parse your results file while looking for "collision report"')

    try:
        collision_report,survivor_report=rest.split('\nthe remaining vehicles are\n')
    except:
        return (False,'Could not parse your results file while looking for "the remaining vehicles are"')


    try:
        N = int(re.match('there are (\d+) vehicles',vehicle_report).groups()[0])
    except:
        return (False,'Could not parse your vehicle report.')

    Collisions=[]

    if collision_report != 'none':
        try:
            for collision in collision_report.splitlines():
                vals = re.match('at (\S+) (\S+) collided with (\S+)',collision).groups()
                vehicles=[vals[1],vals[2]]
                vehicles.sort()
                Collisions.append( (float(vals[0]),*vehicles) )
        except:
         return (False,'Could not parse your collision report.')
    
    Collisions.sort()

    Survivors=[]
    if survivor_report != 'none\n':
        try:
            for survivor in survivor_report.splitlines():
                lic_id,x,y,vx,vy=survivor.split()

                Survivors.append((lic_id,(float(x),float(y)),(float(vx),float(vy))))
        except:
          return (False,'Could not parse your survivor report.')
  
    return (N,Collisions,Survivors)

for tc in LongTestCases:
    Coordfiles[tc]=open(tc+'.coordinates').read().encode()
    Results[tc]=processanswers(open(tc+'.results').read())

for tc in SingleTestCases:
    Coordfiles[tc]=open(tc+'.coordinates').read().encode()
    Results[tc]=processanswers(open(tc+'.results').read())

for version in PairedTestCases:
    Results[version]=[]
    for tc in PairedTestCases[version]:
        Coordfiles[tc]=open(tc+'.coordinates').read().encode()
        Results[version].append(processanswers(open(tc+'.results').read()))


def equivalent_answers(good,maybe):
    Vehicles,Collisions,Survivors = good
    VehiclesMaybe,CollisionsMaybe,SurvivorsMaybe = maybe

    if VehiclesMaybe != Vehicles:
        return False,'Vehicles reported do not match'

    Times = [x for x,y,z in CollisionsMaybe]
    Original = Times[:]
    Times.sort()
    if Times != Original:
        return False,'Your collision times are not in sorted order.'

    if len(Collisions) != len(CollisionsMaybe):
        return False,'The number of collisions reported {} does not match {}'.format(len(CollisionsMaybe),len(Collisions))

    Collisions.sort(key=lambda x: (x[1],x[2]))
    CollisionsMaybe.sort(key=lambda x: (x[1],x[2]))
    
    for coll,coll_maybe in zip(Collisions,CollisionsMaybe):
        time,v1,v2 = coll
        time_m,v1_m,v2_m = coll_maybe
        if v1 != v1_m or v2 != v2_m:
            return False,'The vehicles of {} do not match {}'.format(coll,coll_maybe)
        if (abs(time-time_m)>0.002 and abs(time-time_m)/time >0.0001):
            return False,'The times of {} and {} are not sufficiently close.'.format(coll,coll_maybe)


    if len(Survivors) != len(SurvivorsMaybe):
        return False,'The number of survivors reported does not match'

    for surv,surv_maybe in zip(Survivors,SurvivorsMaybe):
        license,pos,vel = surv
        license_m,pos_m,vel_m = surv_maybe
        if license != license_m:
            return False,'The licenses/ids of {} do not match {}'.format(surv,surv_maybe)
        if abs(pos[0]-pos_m[0]) >0.001 or abs(pos[1]-pos_m[1]) >0.001:
            return False,'The positions of {} and {} are not sufficiently close.'.format(surv,surv_maybe)
        if abs(vel[0]-vel_m[0]) >0.001 or abs(vel[1]-vel_m[1]) >0.001:
            return False,'The velocities of {} and {} are not sufficiently close.'.format(surv,surv_maybe)



    return True,""

class CollisionTestCase(unittest.TestCase):

    def test_single(self):
        """testing collision cases with no ties"""
        for test in SingleTestCases:
            with self.subTest(CASE=test):
                if not SERVER:
                    print('testing',test)
                try:
                    P = subprocess.run("./"+program, input = Coordfiles[test],
                                   stderr=subprocess.PIPE,
                                   stdout=subprocess.PIPE,timeout=TIMEALLOWED)
                except Exception as e:
                    s = "Program {} crashed\n".format(program)
                    s += str(e)
                    self.fail(s)                
                your_output=P.stdout.decode()

                processed = processanswers(your_output)
                if not processed[0]:
                    self.fail('checking {} got the following error:\n{}'.format(test,processed[1]))

                same,diffcase = equivalent_answers(Results[test],processed)
                if not same:
                    self.fail('checking {}\n{}'.format(test,diffcase))

    def test_long(self):
        """testing collision cases with large numbers of vehicles"""
        for test in LongTestCases:
            with self.subTest(CASE=test):
                if not SERVER:
                    print('testing',test)
                try:
                    P = subprocess.run("./"+program, input = Coordfiles[test],
                                   stderr=subprocess.PIPE,
                                   stdout=subprocess.PIPE,timeout=TIMEALLOWEDLONG)
                except Exception as e:
                    s = "Program {} crashed\n".format(program)
                    s += str(e)
                    self.fail(s)                
                your_output=P.stdout.decode()

                processed = processanswers(your_output)
                if not processed[0]:
                    self.fail('checking {} got the following error:\n{}'.format(test,processed[1]))

                same,diffcase = equivalent_answers(Results[test],processed)
                if not same:
                    self.fail('checking {}\n{}'.format(test,diffcase))



    def test_pairs(self):
        """testing collision cases with ties"""
        for test in PairedTestCases:
            for trial in PairedTestCases[test]:
                with self.subTest(CASE=trial):
                    if not SERVER:
                        print('testing',test,trial)
                    try:
                        P = subprocess.run("./"+program, input = Coordfiles[trial],
                                   stderr=subprocess.PIPE,
                                   stdout=subprocess.PIPE,timeout=TIMEALLOWED)
                    except Exception as e:
                        s = "Program {} crashed\n".format(program)
                        s += str(e)
                        self.fail(s)                

                    your_output=P.stdout.decode()


                    processed = processanswers(your_output)
                    if not processed[0]:
                        self.fail('checking {} got the following error:\n{}'.format(trial,processed[1]))
                    passed=False
                    report=''
                    for possible_answer in Results[test]:
                        same, diffcase = equivalent_answers(possible_answer,processed)
                        if same:
                            passed=True
                        else:
                            report = diffcase
                    if not passed:
                        self.fail('checking {} for one of all possible outcomes.\n{}'.format(trial,report))




# GENERIC CHECKER CODE


stylemsgs=[
"\nYour code is super styling!",
"\nWow, what great style you have!",
"\nNice style!",
"\nYou have no lint in your program!"]

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
    """return any errors as a list of strings"""
    errors = []
    passed = []
    loader = unittest.loader.TestLoader()
    tests = loader.loadTestsFromTestCase(CollisionTestCase)
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
        f=open(program_name)
        the_program = f.read()
        f.close()
    except:
        s += 'The program {} does not exist here.\n'.format(orig_program)
        return 'No file',s

    s += '\n---- author check ----\n'
    authors = get_authors(the_program)
    s += 'The authors are: {}\n'.format(" ".join(authors))

    s += '\n---- style check ----\n'

    P_astyle = subprocess.run(['astyle','--formatted','--dry-run',*ASTYLE_OPTIONS],input=the_program.encode(),
           stdout=subprocess.PIPE,stderr=subprocess.PIPE)
    if P_astyle.returncode:
        s += 'astyle returned an error: {}'.format(P_astyle.stderr.decode())
        s += P_astyle.stdout.decode()
        astyle_error = True
    elif P_astyle.stdout.decode().startswith('Formatted'):
        s +='astyle made changes, please use "astyle --style=google --indent=spaces=2" before submitting\n'
        astyle_error = True
    else:
        s += "astyle ok (using options {})\n".format(' '.join(ASTYLE_OPTIONS))
        astyle_error = False

    P_lint = subprocess.run(['cpplint','--filter='+','.join(CPPLINT_OPTIONS),program_name], 
           stderr=subprocess.PIPE)
    if P_lint.returncode:
        s += 'cpplint found problems, as follows:\n'
        s += P_lint.stderr.decode()+"\n"
        cpplint_error = True
    else:
        s += 'cpplint ok (using options {})\n'.format(' '.join(CPPLINT_OPTIONS))
        cpplint_error = False
    
    if not cpplint_error and not astyle_error:
       s += random.choice(stylemsgs)


    s += '\n---- compile check ----\n'
    C = subprocess.run(['g++','--std=gnu++14','-O3',program_name, '-o', program], stderr=subprocess.PIPE)
    if C.returncode:
        s += 'g++ found problems, as follows:'
        s += C.stderr.decode()
        return 'No compile',s
    else:
        s += 'pass\n'

    s += '\n---- includes check ----\n'

    restricted_includes = False
    include_list = get_includes(the_program)
    banned_includes = []
    for x in FORBIDDEN_INCLUDES:
        if x in include_list:
            banned_includes.append(x)

    if banned_includes:
        s += 'You may not include any of the following libraries: {}\n'.format(" ".join(FORBIDDEN_INCLUDES))
        s += 'you have included: {}\n'.format(" ".join(banned_includes))
        restricted_includes = True
    else:
        restricted_includes = False
        s += 'pass\n'


    if restricted_includes:
        return "No program check performed due to restricted include use.",s

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

    s += '\n---- program score ----\n'

    (L,W,C) = code_analysis(program_name)
    
    goal_w = 365
    goal_t = 2.78

    s += "After removing all comments, your code is {} lines, {} words, and {} characters.\n".format(L,W,C)
    s += "The elegance score is based on words, goal of {} words.\n".format(goal_w)


    times = run_it(program,LongTestCases)

    s += 'Your computer is  {:.1f} times faster than the server. Adjusting.\n'.format(your_speed)
    s += "Here are your adjusted run times.\n"
    total =0
    for case in times:
        s += "for {}, time is {:.3f} seconds\n".format(case,times[case])
        total += times[case]

    s += "Total run time is {:.2f}, goal of {:.2f} seconds\n".format(total,goal_t)

    score = (total/goal_t)**2 * (W/goal_w)

    s += "Your estimated score is {:.3f}.\n".format(score)

    return 'Pass',s


if __name__ == '__main__':
    summary,results = main()


    print(results)
    print('\nSummary:', summary)
