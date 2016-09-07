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

# EDS registration request form template

<script language='javascript'>
var doc;
if(document.all){
	doc = document.all;
}else{
	doc = document;
}

function cntest()
{
  doc.request.req.value="dntest";
  doc.request.submit();
}

function tc_id()
{
  doc.request.lookup_tc.disabled = false;
}
function ac_id()
{
  doc.request.lookup_ac.disabled = false;
}

function doform(req)
{
  doc.request.req.value=req;
  doc.request.submit();
}

</script>


<form action="{ROOT}/" method="post" name="request">
  <input type=hidden name=req value=sub>
  <input type=hidden name=xsite value={XSITE}>
  <input type=hidden name=xtime value={XTIME}>
  <input type=hidden name=cn value={CN}>


<p>

# <!-- BDB: old_form -->

# existing authorization request summary
<div class=notice>
<p>
<table border=1 cellpadding=3 cellspacing=1>
  <tr><th colspan=2 align=left>Application</th></tr>
  <tr><td>DNS name</td><td><b>{CN}</b></td></tr>
 <!-- BDB: app_owner -->
  <tr><td>Owner group</td><td>{AG}</td></tr>
 <!-- EDB: app_owner -->
  <tr><th colspan=2 align=left>Authorizations</th></tr>
<!-- BDB: auth_show -->
  <tr><td>{auth_want}</td><td>{auth_status}</td></tr>
<!-- EDB: auth_show -->
</table>
</div>
<p>

#  <tr><td colspan="99">This registration form was last updated by <b>{last_updater}</b>
#  on {last_update}.</td></tr>
# <!-- EDB: old_form -->

<!-- BDB: app_copy -->
<p>
<table border=0 class=policy>
<tr><td colspan=2>Copy information from another form?</td></tr>
<tr><td>Source DNS name:</td><td><input type='text' name='ccn' size='30' value=""></td></tr>
<tr><td></td><td align=right><input type="button" value="Copy" onclick="doform('req_2');"></td></tr>
</table>
<!-- EDB: app_copy -->

<table width="100%" border="0" cellpadding="2">


<tr><td colspan="99"></td></tr>

#
# Questions
#

<tr><td colspan="99"></td></tr>
<tr><td colspan="99"><hr> 
<tr><td colspan="99"></td></tr>

# <tr><td colspan="99">Fields marked with an asterisk and with a yellow background are required.</td></tr>
<tr><td colspan="99"></td></tr>
<tr><td colspan="99"><p><b>Section A: Contact Info</b> </td></tr>

<tr><td colspan="99"></td></tr>


# Question 1

<tr><td colspan="99"><b>Question 1:</b> Technical Contact: The developer,
system administrator
# , or support team 
responsible for the application using the Enterprise Directories. 
Enter a UWNetID and press "Lookup" to fill in values from the UW Whitepages Directory.

</td></tr>

<tr><td width="40" colspan="2"></td>
  <td colspan=99>
  <table>
   <tr><td>UW NetID:</td><td colspan=9><input type='text' name='q1u' size='30' value="{q1u}" onKeyPress="tc_id();">
      <input type="button" name="lookup_tc" {tc_disabled} value="Lookup" onclick="doform('req_2_tc');" >
      </tr>
   <tr><td>Name:</td><td colspan=9><input type='text' name='q1n' size='30' value="{q1n}"></tr>
   <tr><td>Email:</td><td colspan=9><input type='text' name='q1e' size='30' value="{q1e}"></tr>
   <tr><td>Dept:</td><td colspan=9><input type='text' name='q1d' size='30' value="{q1d}"></tr>
   <tr><td>Title:</td><td colspan=9><input type='text' name='q1t' size='30' value="{q1t}"></tr>
   <tr><td>Phone:</td><td colspan=9><input type='text' name='q1p' size='30' value="{q1p}"></tr>
 </table>
 </td></tr>

<tr><td colspan="99"></td></tr>

# Question 2

<tr><td colspan="99"><p><b>Question 2:</b> Administrative Contact: The 
manager or engineer responsible for the business operations supported by
the application. Please enter a different
contact from Technical Contact above, perhaps a supervisor or alternative
application administrator.
Enter a UWNetID and press "Lookup" to fill in values from the UW Whitepages Directory.
</td></tr>

<tr><td width="40" colspan="2"></td>
  <td colspan=99>
  <table>
   <tr><td>UW NetID:</td><td colspan=9><input type='text' name='q2u' size='30' value="{q2u}" onKeyPress="ac_id();">
      <input type="button" name="lookup_ac" {ac_disabled} value="Lookup" onclick="doform('req_2_ac');" >
      </tr>
   <tr><td>Name:</td><td colspan=9><input type='text' name='q2n' size='30' value="{q2n}"></tr>
   <tr><td>Email:</td><td colspan=9><input type='text' name='q2e' size='30' value="{q2e}"></tr>
   <tr><td>Dept:</td><td colspan=9><input type='text' name='q2d' size='30' value="{q2d}"></tr>
   <tr><td>Title:</td><td colspan=9><input type='text' name='q2t' size='30' value="{q2t}"></tr>
   <tr><td>Phone:</td><td colspan=9><input type='text' name='q2p' size='30' value="{q2p}"></tr>
 </table>
 </td></tr>

# Question 3

<tr><td colspan="99"><p><b>Question 3:</b> Support team email: 
The team contact email address will be added to <b>directory-users@uw.edu</b>
for announcements of outages and service changes.
</td></tr>

<tr><td width="40" colspan="2"></td>
  <td colspan=99>
  <table>
   <tr><td>Email:</td><td colspan=9><input type='text' name='q1s' size='30' value="{q1s}"></td></tr>
 </table>
 </td></tr>

<tr><td colspan="99"></td></tr>

<tr><td colspan="99"></td></tr>

<tr><td colspan="99"><p><b>Section B: Application Info</b></td></tr>

<tr><td colspan="99"></td></tr>

# Question 4

<tr><td colspan="99"><p><b>Question 4.</b> What is the name of your application? (E.g. "MyUW" or "Dept X Budget Tracking")
</td></tr>

<tr><td width="40" colspan="2"></td><td valign=top colspan="99"><input type='text' name='q3' size='40' value="{q3}"></td></tr>
<tr><td colspan="99"></td></tr>

# Question 5

<tr><td colspan="99"><p><b>Question 5.</b> What is the URL of your application's supporting
documentation? 
</td></tr>

<tr><td width="40" colspan="2"></td><td colspan="99"><input type='text' name='q4' size='40' value="{q4}"></td></tr>

<tr><td colspan="99"></td></tr>


# Question 6

<tr><td colspan="99"><p><b>Question 6.</b> What type of application is it?
</td></tr>

<tr><td width="20"></td><td align="right" valign=top><input type="radio" name="q5" value="q5_admin" {q5_admin_checked}></td>
<td colspan="99" valign=top>Central Business Unit Application: supporting processes managed by
UW central business units, such as HR, Academic Affairs, Budget, Health
&amp; Safety, Facilities, C&amp;C, etc.</td></tr>

<tr><td width="20"></td><td align="right" valign=top><input type="radio" name="q5" value="q5_dept" {q5_dept_checked}></td>
<td colspan="99" valign=top>Departmental Application: supporting official UW processes in
<ul>
<li><a href="http://www.washington.edu/home/departments/departments.html">Colleges, Schools, Departments, and Degree Programs</a>,
<li><a href="http://www.washington.edu/home/departments/labs-centers.html">Labs, Centers, Programs, and Projects</a>,
<li>etc.
</ul>
</td></tr>

<tr><td width="20"></td><td align="right"><input type="radio" name="q5" value="q5_other" {q5_other_checked}></td>
<td colspan="99">Other</td></tr>

<tr><td width="20"></td><td align="right"><input type="radio" name="q5" value="q5_xxx" {q5_xxx_checked}></td>
<td colspan="99">Don't know</td></tr>

<tr><td colspan="99"></td></tr>

# Question 7

<tr><td colspan="99"><p><b>Question 7.</b> What authorizations
 does your application require?
</td></tr>

<tr><td width="20"></td><td></td><td colspan=2 nowrap>Person Directory / Person Web Service<br>
      <a href="https://wiki.cac.washington.edu/display/infra/Person+Directory+Attribute+Reference">Attribute Sets</a></td>
    <td width="20"></td><td colspan=2>Groups Directory<br>memberships</td>
##  <td width="20"></td><td colspan=2>UW Windows Infrastructure Data<br>course memberships</td>
</tr>

<tr><td width="20"></td><td align="right"><input type="checkbox" name="q6" value="q6_basic" {q6_basic_checked}></td>
     <td colspan=1>Basic</td>
   <td width="20"></td><td align="right"><input type="checkbox" name="q6" value="q6_groups" {q6_groups_checked}></td>
     <td colspan=1>Basic</td>
##   <td width="20"></td><td align="right"><input type="checkbox" name="q6" value="q6_uwwib" {q6_uwwib_checked}></td>
##     <td colspan=99>Basic</td>
</tr>

<tr><td width="20"></td><td align="right"><input type="checkbox" name="q6" value="q6_student" {q6_student_checked}></td>
<td colspan=1>Student</td>
   <td width="20"></td><td align="right"><input type="checkbox" name="q6" value="q6_courses" {q6_courses_checked}></td>
   <td colspan=1>Courses</td>
##   <td width="20"></td><td align="right"><input type="checkbox" name="q6" value="q6_uwwic" {q6_uwwic_checked}></td>
##     <td colspan=99>Courses</td>
</tr>

<tr><td width="20"></td><td align="right"><input type="checkbox" name="q6" value="q6_employee" {q6_employee_checked}></td>
<td colspan=1>Employee</td></tr>

<tr><td width="20"></td><td align="right"><input type="checkbox" name="q6" value="q6_alumni" {q6_alumni_checked}></td>
<td colspan=1>Alumni</td></tr>

<tr><td colspan="99"></td></tr>




# Question 8

<tr><td colspan="99"><p><b>Question 8.</b> Please describe your application.</td></tr>
<tr><td colspan="99"></td></tr>

<tr><td width="40" colspan="2"></td><td colspan="99">What is the purpose of the application?</td><td>
<tr><td colspan="99"></td></tr>
<tr><td width="40" colspan="2"></td><td colspan="99"><textarea name="q7a" rows="4" cols="50" wrap="virtual">{q7a}</textarea>
</td></tr>
<tr><td colspan="99"></td></tr>
<tr><td width="40" colspan="2"></td><td colspan="99">What specific attributes will it use?</td><td>
<tr><td colspan="99"></td></tr>
<tr><td width="40" colspan="2"></td><td colspan="99"><textarea name="q7b" rows="4" cols="50" wrap="virtual">{q7b}</textarea>
</td></tr>
<tr><td colspan="99"></td></tr>
<tr><td width="40" colspan="2"></td><td colspan="99">What population of users does it serve?</td><td>
<tr><td colspan="99"></td></tr>
<tr><td width="40" colspan="2"></td><td colspan="99"><textarea name="q7c" rows="4" cols="50" wrap="virtual">{q7c}</textarea>
</td></tr>
<tr><td colspan="99"></td></tr>
<tr><td width="40" colspan="2"></td><td colspan="99">How often will it access the directory?  </td></tr>
<tr><td width="40" colspan="2"></td><td colspan="99"><textarea name="q7d" rows="4" cols="50" wrap="virtual">{q7d}</textarea>
</td></tr>
<tr><td colspan="99"></td></tr>
<tr><td width="40" colspan="2"></td><td colspan="99">What LDAP client technology will the application use to access the directory?
Enter 'PWS' if you will be using the Person Web Service.</td></tr>
<tr><td width="40" colspan="2"></td><td colspan="99"><textarea name="q8" rows="4" cols="50" wrap="virtual">{q8}</textarea>
</td></tr>


<tr><td colspan="99"></td></tr>


# Question 9

<tr><td colspan="99"><p><b>Question 9.</b>What business or end-user impact would occur if your application
temporarily could not access enterprise data (as might happen during an
unexpected service outage). How critical to UW business functions would this impact be?
</td></tr>

<tr><td width="40" colspan="2"></td><td colspan="99"><textarea name="q8n" rows="5" cols="50" wrap="virtual">{q8n}</textarea>
</td></tr>

<tr><td colspan="99"></td></tr>



<tr><td colspan="99"></td></tr>

<tr><td colspan="99"><b>Section C: Data Protection</b></td></tr>

<tr><td colspan="99"></td></tr>



# Question 10

<tr><td colspan="99"></td></tr>
<tr><td colspan="99"><p><b>Question 10.</b>How does your application ensure that Enterprise data is not
accessible or viewable by unauthorized parties? How are users
authenticated and authorized for access to the application?
</td></tr>
<tr><td width="40" colspan="2"></td><td colspan="99"><p class="red"><textarea name="q9" rows="5" cols="50" wrap="virtual">{q9}</textarea></td></tr>

<tr><td colspan="99"></td></tr>


# Question 11


<tr><td colspan="99"><p><b>Question 11.</b>What are the hostnames of the (test and production) machines
on which the calling application will run or store enterprise data.
</td></tr>
<tr><td width="40" colspan="2"></td><td colspan="99"><p class="red"><textarea name="q10" rows="5" cols="50" wrap="virtual">{q10}</textarea></td></tr>

<tr><td colspan="99"></td></tr>


# Question 12


<tr><td colspan="99"><p><b>Question 12.</b>How are these hosts protected from
unauthorized remote or local access to application-stored Enterprise data,
private keys used to access the directory, or other means of unauthorized
viewing of Enterprise data?
</td></tr>
<tr><td width="40" colspan="2"></td><td colspan="99"><p class="red"><textarea name="q11" rows="5" cols="50" wrap="virtual">{q11}</textarea></td></tr>

<tr><td colspan="99"></td></tr>


# Question 13


<tr><td colspan="99"><p><b>Question 13.</b>If your application accesses Student attributes,
in what ways might it expose private student data? Does it use
student names? Does it display lists of students (e.g. through
directory lists or "people-picker" menus)?
</td></tr>
<tr><td width="40" colspan="2"></td><td colspan="99"><p class="red"><textarea name="q12" rows="5" cols="50" wrap="virtual">{q12}</textarea></td></tr>




<tr><td colspan="99"></td></tr>
<tr><td colspan="99"><p><b>Section D: Other</b></td></tr>



# Question 14

<tr><td colspan="99"></td></tr>
<tr><td colspan="99"><p><b>Question 14.</b> Please enter any comments or additional information
you'd like to send along with this registration.</td></tr>
<tr><td width="40" colspan="2"></td><td colspan="99"><textarea name="q13" rows="5" cols="50" wrap="virtual">{q13}</textarea>
</td></tr>


<tr><td colspan="99"></td></tr>
<tr class=code>
 <td colspan="99" >
  &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;
  <!-- <input type="submit" value="Submit" > -->
  <input type="button" value="Submit" onclick="doform('sub');" >
  </td>
</tr>
<br>

</table>

</form>

# end of the form
