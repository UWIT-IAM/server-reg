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

# EDS registration: admin detail display

<script language='javascript'>
var doc;
if(document.all){
   doc = document.all;
} else {
   doc = document;
}

function actb()
{
  doc.authorize.actbutton.disabled = false;
}

function do_auth(cmd)
{
  doc.authorize.req.value=cmd;
  doc.authorize.submit();
}
</script>

<table width="100%" border="0" cellpadding="4">


<tr><td colspan="99"></td></tr>
<tr><td colspan="99"><p><b>Application DNS name:</b> <b><tt>{CN}</tt></b>

<p>
# <div class=policy>
# This service's attribute set authorizations


<form name=authorize action={ROOT}/admin/ method=post>
  <input type=hidden name=req value=xxxx>
  <input type=hidden name=authset value=xxxx>
  <input type=hidden name=xsite value={XSITE}>
  <input type=hidden name=xtime value={XTIME}>
  <input type=hidden name=cn value={CN}>
  <input type=hidden name=q_cn value={CN}>

<p class=indent>

<table border=1 cellpadding=3 cellspacing=3 class=indent>
<tr><th>Authorization</th><th>Req'd</th><th>App'd</th><th>On-ldap</th><th colspan=9>Authorized actions<th></tr>
<!-- BDB: auth_show -->
  <tr>
    <td>{auth_name}</td>
    <td class={auth_req_class}>{auth_req}</td>
    <td class={auth_app_class}>{auth_app}</td>
    <td class={auth_ldap_class}>{auth_ldap}</td>

<!-- BDB: grant -->
<td align=left>
<input type=checkbox name=auth_action value="grant {auth_name}" onclick="actb();">Grant {auth_name} access</td>
</td>
<!-- EDB: grant -->
<!-- BDB: revoke -->
<td align=left>
<input type=checkbox name=auth_action value="revoke {auth_name}" onclick="actb();">Revoke {auth_name} access</td>
</td>
<!-- EDB: revoke -->
<!-- BDB: ldapgrant -->
<td align=left>
<input type=checkbox name=auth_action value="ldapgrant {auth_name}" onclick="actb();">Fix {auth_name} access</td>
</td>
<!-- EDB: ldapgrant -->
<!-- BDB: ldaprevoke -->
<td align=left>
<input type=checkbox name=auth_action value="ldaprevoke {auth_name}" onclick="actb();">Fix {auth_name} access</td>
</td>
<!-- EDB: ldaprevoke -->

  </tr>
<!-- EDB: auth_show -->

<!-- BDB: auth_button -->
  <tr><td colspan=4></td>
  <td align=center>
    <input type=button name="actbutton" value="Perform actions" onClick="do_auth('action');" disabled>
    </td>
  </tr>
<!-- EDB: auth_button -->

</table>

</form>
</p>
# </div>
<p>

  This registration form was last updated by <b>{last_updater}</b>
  on {last_update}.

<p>


#
# Questions
#

<tr><td colspan="99"></td></tr>
<tr><td colspan="99"><hr> 
<tr><td colspan="99"></td></tr>

<tr><td colspan="99"></td></tr>
<tr><td colspan="99"><p><b>Section A: Contact Info</b> </td></tr>

<tr><td colspan="99"></td></tr>


# Question 1

<tr><td colspan="99"><p><b>Question 1:</b> Technical Contact: 
</td></tr>

<tr><td width="40" colspan="2"></td>
  <td colspan=99>
  <table>
   <tr><td>NetID:</td><td class=response>{q1u}</tr>
   <tr><td>Name:</td><td class=response>{q1n}</tr>
   <tr><td>Email:</td><td class=response>{q1e}</tr>
   <tr><td>Dept:</td><td class=response>{q1d}</tr>
   <tr><td>Title:</td><td class=response>{q1t}</tr>
   <tr><td>Phone:</td><td class=response>{q1p}</tr>
 </table>
 </td></tr>

<tr><td colspan="99"></td></tr>

# Question 2

<tr><td colspan="99"><p><b>Question 2:</b> Administrative Contact: 
</td></tr>

<tr><td width="40" colspan="2"></td>
  <td colspan=99>
  <table>
   <tr><td>NetID:</td><td class=response>{q2u}</tr>
   <tr><td>Name:</td><td class=response>{q2n}</tr>
   <tr><td>Email:</td><td class=response>{q2e}</tr>
   <tr><td>Dept:</td><td class=response>{q2d}</tr>
   <tr><td>Title:</td><td class=response>{q2t}</tr>
   <tr><td>Phone:</td><td class=response>{q2p}</tr>
 </table>
 </td></tr>

# Question 3

<tr><td colspan="99"><p><b>Question 3:</b> Support team email:
</td></tr>

<tr><td width="40" colspan="2"></td>
  <td colspan=99>
  <table>
   <tr><td>Email:</td><td class=response>{q1s}</tr>
 </table>
 </td></tr>

<tr><td colspan="99"></td></tr>

<tr><td colspan="99"></td></tr>

<tr><td colspan="99"><p><b>Section B: Application Info</b></td></tr>

<tr><td colspan="99"></td></tr>

# Question 4

<tr><td colspan="99"><p><b>Question 4.</b> What is the name of your application? 
</td></tr>

<tr><td width="40" colspan="2"></td><td colspan="99" class=response>{q3}</td></tr>
<tr><td colspan="99"></td></tr>

# Question 5

<tr><td colspan="99"><p><b>Question 5.</b> What is the URL of your application's supporting
documentation? 
</td></tr>

<tr><td width="40" colspan="2"></td><td colspan="99" class=response>{q4}</td></tr>

<tr><td colspan="99"></td></tr>


# Question 6

<tr><td colspan="99"><p><b>Question 6.</b> What type of application is it?
</td></tr>

<tr><td width="40"></td><td colspan="99" valign=top class=response>
<!-- BDB: q5_admin_checked -->
Central Business Unit Application
<!-- EDB: q5_admin_checked -->
<!-- BDB: q5_dept_checked -->
Departmental Application
<!-- EDB: q5_dept_checked -->
<!-- BDB: q5_xxx_checked -->
Don't know
<!-- EDB: q5_xxx_checked -->
<!-- BDB: q5_other_checked -->
Other
<!-- EDB: q5_other_checked -->
</td></tr>

<tr><td colspan="99"></td></tr>

# Question 7

<tr><td colspan="99"><p><b>Question 7.</b> What <a href="http://www.washington.edu/computing/eds/person.html#data">Attribute Sets</a>
 does your application require?
</td></tr>

<tr><td width="40"></td><td colspan=99 class=response>
<!-- BDB: q6_basic_checked -->
PDS: Basic<br>
<!-- EDB: q6_basic_checked -->
<!-- BDB: q6_student_checked -->
PDS: Student<br>
<!-- EDB: q6_student_checked -->
<!-- BDB: q6_employee_checked -->
PDS: Employee<br>
<!-- EDB: q6_employee_checked -->
<!-- BDB: q6_alumni_checked -->
PDS: Alumni<br>
<!-- EDB: q6_alumni_checked -->
<!-- BDB: q6_groups_checked -->
GDS: Basic<br>
<!-- EDB: q6_groups_checked -->
<!-- BDB: q6_courses_checked -->
GDS: Courses<br>
<!-- EDB: q6_courses_checked -->
</td></tr>


<tr><td colspan="99"></td></tr>

# Question 8

<tr><td colspan="99"><p><b>Question 8.</b> Please describe your application. 
</td></tr>

<tr><td colspan="2"></td><td>What is the purpose of the application?
</td></tr>
<tr><td width="40" colspan="2"></td><td colspan="99" class=response>{q7a}
</td></tr>
<tr><td colspan="2"></td><td>What specific attributes will it use?
</td></tr>
<tr><td width="40" colspan="2"></td><td colspan="99" class=response>{q7b}
</td></tr>
<tr><td colspan="2"></td><td>What population of users does it serve?
</td></tr>
<tr><td width="40" colspan="2"></td><td colspan="99" class=response>{q7c}
</td></tr>
<tr><td colspan="2"></td><td>How often will it access the directory?
</td></tr>
<tr><td width="40" colspan="2"></td><td colspan="99" class=response>{q7d}
</td></tr>
<tr><td colspan="2"></td><td>What LDAP client technology will the application use?
</td></tr>
<tr><td width="40" colspan="2"></td><td colspan="99" class=response>{q8}
</td></tr>

<tr><td colspan="99"></td></tr>



# Question 9

<tr><td colspan="99"><b>Question 9.</b>What business or end-user impact would occur?
</td></tr>

<tr><td width="40" colspan="2"></td><td colspan="99" class=response>{q8n}
</td></tr>

<tr><td colspan="99"></td></tr>

<tr><td colspan="99"></td></tr>


<tr><td colspan="99"><b>Section C: Data Protection</b></td></tr>

<tr><td colspan="99"></td></tr>




# Question 10

<tr><td colspan="99"></td></tr>
<tr><td colspan="99"><p><b>Question 10.</b> How does your application ensure that Enterprise data is not
accessible or viewable by unauthorized parties? 
</td></tr>
<tr><td width="40" colspan="2"></td><td colspan="99" class=response>{q9}
</td></tr>

<tr><td colspan="99"></td></tr>


# Question 11


<tr><td colspan="99"><p><b>Question 11.</b> What are the hostnames of the (test and production) machines
on which Person data will be stored for use by this application?
</td></tr>
<tr><td width="40" colspan="2"></td><td colspan="99" class=response>{q10}</td></tr>

<tr><td colspan="99"></td></tr>


# Question 12


<tr><td colspan="99"><p><b>Question 12.</b> How are these hosts protected from unauthorized remote or
local access to application-stored Person data?
</td></tr>
<tr><td width="40" colspan="2"></td><td colspan="99" class=response>{q11}</td></tr>

<tr><td colspan="99"></td></tr>


# Question 13


<tr><td colspan="99"><p><b>Question 13.</b> If your application uses data from the Student attribute
set, in what ways might it expose private student data? Does it use
student names? Does it display lists of students (e.g. through
directory lists or "people-picker" menus)?
</td></tr>
<tr><td width="40" colspan="2"></td><td colspan="99" class=response>{q12}</td></tr>



<tr><td colspan="99"></td></tr>
<tr><td colspan="99"><p><b>Section D: Other</b></td></tr>
<tr><td colspan="99"></td></tr>



# Question 14

<tr><td colspan="99"></td></tr>
<tr><td colspan="99"><p><b>Question 14.</b> Please enter any comments or additional information
you'd like to send along with this registration.</td></tr>
<tr><td width="40" colspan="2"></td><td colspan="99" class=response>{q13}
</td></tr>


<tr><td colspan="99"></td></tr>
<br>

</table>


# end of the form
