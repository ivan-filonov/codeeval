#"""Sample code to read in test cases:

import sys
test_cases = open(sys.argv[1], 'r')
te=('''***----*---*-----*--****--**--***---**--
---*--**--*--*--**--*----*--*----*-*----
-**----*--****---*--***---***--**--***--
---*---*-----*---*-----*----*-*----*--*-
***---***----*--***-***---**--****--**--
----------------------------------------''',
'''--*---*-----*---*---***----*--***--****--**--
-**--*--*--**--*--*----*--**-----*-*----*----
--*--****---*--****--**----*---**--***--***--
--*-----*---*-----*-*------*-----*----*-*--*-
-***----*--***----*-****--***-***--***---**--
---------------------------------------------''',
'''-**----*---**----*----*---**--****--**--
*--*--**--*--*--**---**--*--*----*-*--*-
*--*---*--*--*---*----*---***---*--*--*-
*--*---*--*--*---*----*-----*--*---*--*-
-**---***--**---***--***--**---*----**--
----------------------------------------''',
'''***--****---*---**--***---**----*---**--***---**---*---
---*----*--**--*--*----*-*--*--**--*--*----*-*--*-*--*-
-**----*----*---**---**---**----*---**---**---**--****-
*-----*-----*--*--*-*----*--*---*--*--*-*----*--*----*-
****--*----***--**--****--**---***--**--****--**-----*-
-------------------------------------------------------''',
'''-*----**----*--****---*---**--***--***---*---***--
*--*-*--*--**--*-----**--*-------*----*-*--*----*-
****--**----*--***----*--***---**---**--****--**--
---*-*--*---*-----*---*--*--*-*-------*----*-*----
---*--**---***-***---***--**--****-***-----*-****-
--------------------------------------------------''')
tv='''3.1415926
1.41421356
01-01-1970
2.7182818284
4 8 15 16 23 42'''.splitlines()
cm='''-**----*--***--***---*---****--**--****--**---**--
*--*--**-----*----*-*--*-*----*-------*-*--*-*--*-
*--*---*---**---**--****-***--***----*---**---***-
*--*---*--*-------*----*----*-*--*--*---*--*----*-
-**---***-****-***-----*-***---**---*----**---**--
--------------------------------------------------'''.splitlines()
def add(i,r,w=5):
	for l in range(len(cm)):
		r[l]=r[l]+cm[l][i*w:i*w+w]
def newr(): return['']*6
def bigline(s):
	r=['']*6
	d=[int(x) for x in s if x.isdigit()]
	for i in d: add(i,r)
	for z in r: print(z)

for test in test_cases:
    # ignore test if it is an empty line
    # 'test' represents the test case, do something with it
    # ...
    # ...
    test=test.strip()
    if len(test)==0:
        continue
    bigline(test)

test_cases.close()
#"""
