#  ========================================================================
#  Copyright (c) 2006-2007 The University of Washington
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

# EDS registration request log and email(parallels the form)

Application DNS name: {CN}
Provided by: {remote_user}
On: {show_date}

  

Section A: Contact Info


Question 1: Technical Contact: 

   NetID:  {q1u}
   Name:   {q1n}
   Email:  {q1e}
   Dept:   {q1d}
   Title:  {q1t}
   Phone:  {q1p}



Question 2: Administrative Contact: 

   NetID:  {q2u}
   Name:   {q2n}
   Email:  {q2e}
   Dept:   {q2d}
   Title:  {q2t}
   Phone:  {q2p}


Question 3: Support team email:

   EMail:  {q1s}


Section B:  Application Info


Question 4. Name of your application?

   {q3}


Question 5.  URL of your application's supporting documentation?

   {q4}


Question 6. What type of application is it?

   {q5}


Question 7. What Attribute Sets does your application require?

   {q6}


Question 8. Describe your application.

What is the purpose of the application?

   {q7a}

What specific attributes will it use?

   {q7b}

What population of users does it serve?

   {q7c}

How often will it access the directory?

   {q7d}

What LDAP client technology will the application use?

   {q8}


Question 9. What business or end-user impact would occur if
   your application temporarily could not access enterprise
   data. How critical to UW business functions would this impact be?

   {q8n}


Section C: Data Protection


Question 10. How does your application ensure that Enterprise
   data is not accessible or viewable by unauthorized parties?
   How are users authenticated and authorized for access to the
   application?

   {q9}


Question 11. What are the hostnames of the (test and production)
   machines on which the calling application will run or store
   enterprise data.

   {q10}


Question 12. How are these hosts protected from unauthorized remote
   or local access to application-stored Enterprise data, private keys
   used to access the directory, or other means of unauthorized
   viewing of Enterprise data?

   {q11}


Question 13. If your application accesses Student attributes, in what
   ways might it expose private student data? Does it use student names?
   Does it display lists of students (e.g. through directory lists or
   "people-picker" menus)?

   {q12}


Section D: Other


Question 14. Additional information

   {q13}


# end of the form
