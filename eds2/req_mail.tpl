#  ========================================================================
#  Copyright (c) 2006 The University of Washington
# 
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
# 
#      http://www.apache.org/licenses/LICENSE-2.0
# 
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
#  ========================================================================
# 

# EDS registration mailer  (parallels the req_2 form)
# this is the text that goes to iam-support as notification


EDS application information for: {CN}
Provided by: {remote_user}
On: {show_date}

----------------------------------------

Questions 1-2 Contact Info


Question 1: Technical Contact: 

   Name:   {q1n}
   Email:  {q1e}
   Dept:   {q1d}
   Title:  {q1t}
   Phone:  {q1p}



Question 2: Administrative Contact: 

   Name:   {q2n}
   Email:  {q2e}
   Dept:   {q2d}
   Title:  {q2t}
   Phone:  {q2p}


Questions 3-8 Application Info:</b></td></tr>


Question 3. Application name? 

   {q3}


Question 4. Application URL

   {q4}


Question 5. What type of application is it?

   {q5}


Question 6. What Attribute Sets does your application require?

   {q6}


Question 7. Application description

   {q7}


Question 8. LDAP client technology

   {q8}


Questions 9-12 Data Protection


Question 9. How ensure that Person data is not accessible?

   {q9}


Question 10. Hostnames

   {q10}


Question 11. How hosts protected?

   {q11}


Question 12. Student attribute date protection

   {q12}


Questions 13 Other


Question 13. Additional information

  {q13}


# end of the form
