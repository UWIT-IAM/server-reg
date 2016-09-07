#  ========================================================================
#  Copyright (c) 2005 The University of Washington
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

# Pubcookie registration request form template

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

function checkkey(l,r,e)
{
  var code = 0;
  if (window.event) code = window.event.keyCode;
  else if (e) code = e.which;
  if (code==13) cntest(); 
}

function doform()
{
  doc.request.req.value="sub";
  doc.request.submit();
}

</script>


<form action="{ROOT}/" method="post" name="request">
  <input type=hidden name=req value=sub>
  <input type=hidden name=xseq value={XSEQ}>


<table width="100%" border="0" cellpadding="4">


<tr><td colspan="3"></td></tr>
<tr><td colspan="3"><p><b>Domain name:</b>
   Please enter your server's DNS domain name.  This should be the
   same name that appears in your server's URL and in your server's
   SSL certificate (in the Common Name field).
 </td></tr>
  
<tr><td width="40" colspan="2"></td><td>
   <input type="text" name="cn" size="40" onKeyPress="checkkey('','',event);">
     </td></tr>
<p>
<tr><td colspan="3"><p>
You must be the recognized 
   owner of the domain in order for us to process your application.
   If you're not sure test its ownership.
  </td></tr>

<tr><td width="40" colspan="2"></td><td>
  <input type="button" value="Test the domain" onclick="cntest();" >
     </td></tr>

<tr><td colspan="3"></td></tr>
<tr><td colspan="3"><hr> 
<tr><td colspan="3"></td></tr>
<!-- BDB: preapp -->
<tr><td colspan="3"><p><b>Note:</b> You are pre-approved 
for any valid registration request.  We appreciate your answers
to these questions nonetheless. </td></tr>
<!-- EDB: preapp -->

<tr><td colspan="3"></td></tr>
<tr><td colspan="3"><p><b>Questions 1-3 Authentication Requirements:</b> </td></tr>


<tr><td colspan="3"></td></tr>
<tr><td colspan="3"><p><b>Question 1:</b> Please describe the application or resource on this server 
that will be protected by Pubcookie, including whether any <a
href="http://www.washington.edu/computing/rules/privacypolicy.html">personally identifiable information</a> will be involved.</td></tr>

<tr><td width="40" colspan="2"></td><td><p class="red"><textarea name="q1" rows="5" cols="50" wrap="virtual" ></textarea></td></tr>


<tr><td colspan="3"></td></tr>
<tr><td colspan="3"><p><b>Question 2.</b> Are you already authenticating users to resources on this server? If so, how?</td></tr>

<tr><td width="40" colspan="2"></td><td><textarea name="q2" rows="5" cols="50" wrap="virtual" ></textarea>
</td></tr>


<tr><td colspan="3"></td></tr>
<tr><td colspan="3"><p><b>Question 3.</b> Do you plan to use 2-factor authentication?</td></tr>

<tr><td width="20"></td><td align="right"><p><input type="radio" name="q3" value="Yes"></td>
<td><p>Yes</td></tr>

<tr><td width="20"></td><td align="right"><p><input type="radio" name="q3" value="No"></td>
<td><p>No</td></tr>


<tr><td colspan="3"></td></tr>
<tr><td colspan="3"><p><b>Questions 4-7 User Population</b></td></tr>


<tr><td colspan="3"></td></tr>
<tr><td colspan="3"><p><b>Question 4.</b> How many different users do you expect to access Pubcookie-protected resources on this 
server?</td></tr>

<tr><td width="20"></td><td align="right"><p><input type="radio" name="q4" value="<10"></td>
<td><p>Fewer than 10 users</td></tr>

<tr><td width="20"></td><td align="right"><p><input type="radio" name="q4" value="10-100"></td>
<td><p>10-100 users</td></tr>

<tr><td width="20"></td><td align="right"><p><input type="radio" name="q4" value="100-1000"></td>
<td><p>101-1,000 users</td></tr>

<tr><td width="20"></td><td align="right"><p><input type="radio" name="q4" value=">1000"></td>
<td><p>More than 1,000 users</td></tr>


<tr><td colspan="3"></td></tr>
<tr><td colspan="3"><p><b>Question 5.</b> On a busy day, how many user logins do you expect to initiate on this server?</td></tr>

<tr><td width="20"></td><td align="right"><p><input type="radio" name="q5" value="<10"></td>
<td><p>Fewer than 10 logins</td></tr>

<tr><td width="20"></td><td align="right"><p><input type="radio" name="q5" value="10-100"></td>
<td><p>10-100 logins</td></tr>

<tr><td width="20"></td><td align="right"><p><input type="radio" name="q5" value="100-1000"></td>
<td><p>101-1,000 logins</td></tr>

<tr><td width="20"></td><td align="right"><p><input type="radio" name="q5" value=">1000"></td>
<td><p>More than 1,000 logins</td></tr>


<tr><td colspan="3"></td></tr>
<tr><td colspan="3"><p><b>Question 6.</b> Do all of your potential users have UW NetIDs? If not, how do plan to handle the
users who do not already have them?</td></tr>

<tr><td width="40" colspan="2"></td><td><textarea name="q6" rows="5" cols="50" wrap="virtual" ></textarea>


<tr><td colspan="3"></td></tr>
<tr><td colspan="3"><p><b>Question 7.</b> What affiliations with the university do your users have?</td></tr>

<tr><td width="20"></td><td align="right"><input type="checkbox" name="q7" value="Student"></td>
<td><p>student</td></tr>

<tr><td width="20"></td><td align="right"><input type="checkbox" name="q7" value="Faculty"></td>
<td><p>faculty</td></tr>

<tr><td width="20"></td><td align="right"><input type="checkbox" name="q7" value="Staff"></td>
<td><p>staff</td></tr>

<tr><td width="20"></td><td align="right"><input type="checkbox" name="q7" value="Alumni"></td>
<td><p>alumni</td></tr>

<tr><td width="20"></td><td align="right"><input type="checkbox" name="q7" value="UW Medicine"></td>
<td><p>UW Medicine</td></tr>

<tr><td width="20"></td><td align="right"><input type="checkbox" name="q7" value="Other"></td>
<td><p>other <input type='text' name='q7' size='30' onfocus='doc.request.q7[5].checked = true;'></td></tr>


<tr><td colspan="3"></td></tr>
<tr><td colspan="3"><p><b>Questions 8-9 Authorization Requirements:</b> together the UW NetID "weblogin" service
and Pubcookie authenticate users. That is, they establish the identity of users. The following questions concern authorization and
what each user is allowed to do on your server.</td></tr>


<tr><td colspan="3"></td></tr>
<tr><td colspan="3"><p><b>Question 8.</b> How do you plan to handle authorization?</td></tr>

<tr><td width="40" colspan="2"></td><td><textarea name="q8" rows="5" cols="50" wrap="virtual" ></textarea>
</td></tr>


<tr><td colspan="3"></td></tr>
<tr><td colspan="3"><p><b>Question 9.</b> Do you need to distinguish one UW affiliation from another (e.g. students 
from staff from faculty) or check uw course enrollment?</td></tr>

<tr><td width="20"></td><td align="right"><p><input type="radio" name="q9" value="Yes"></td>
<td><p>Yes</td></tr>

<tr><td width="20"></td><td align="right"><p><input type="radio" name="q9" value="No"></td>
<td><p>No</td></tr>


<tr><td colspan="3"></td></tr>
<tr><td colspan="3"><p><b>Questions 10-11 Server Configuration</b></td></tr>


<tr><td colspan="3"></td></tr>
<tr><td colspan="3"><p><b>Question 10.</b> On what platform is this server running?</td></tr>

<tr><td width="20"></td><td align="right"><p><input type="radio" name="q10" value="*BSD"></td>
<td><p>*BSD</td></tr>

<tr><td width="20"></td><td align="right"><p><input type="radio" name="q10" value="Linux"></td>
<td><p>Linux</td></tr>

<tr><td width="20"></td><td align="right"><p><input type="radio" name="q10" value="AIX"></td>
<td><p>IBM AIX</td></tr>

<tr><td width="20"></td><td align="right"><p><input type="radio" name="q10" value="Sun Solaris"></td>
<td><p>Sun Solaris</td></tr>

<tr><td width="20"></td><td align="right"><p><input type="radio" name="q10" value="Mac OS X"></td>
<td><p>Mac OS X</td></tr>

<tr><td width="20"></td><td align="right"><p><input type="radio" name="q10" value="Other Unix"></td>
<td><p>Other Unix <input type='text' name='q10' size='30' onfocus='doc.request.q10[5].checked = true;'></td></tr>

<tr><td width="20"></td><td align="right"><p><input type="radio" name="q10" value="Windows 2000"></td>
<td><p>Microsoft Windows 2000</td></tr>

<tr><td width="20"></td><td align="right"><p><input type="radio" name="q10" value="Windows Server 2003"></td>
<td><p>Microsoft Windows Server 2003</td></tr>

<tr><td width="20"></td><td align="right"><p><input type="radio" name="q10" value="other windows"></td>
<td><p>Other Microsoft Windows <input type='text' name='q10' size='30' onfocus='doc.request.q11[8].checked = true;'></td></tr>


<tr><td colspan="3"></td></tr>
<tr><td colspan="3"><p>
<b>Question 11.</b>
We suggest that you have an SSL certificate signed by one of our certificate providers,
as described in our <a href="https://wiki.cac.washington.edu/x/KDaoAQ">Certificate Services</a>.
If you have a certificate from some other CA,
please enter the name of the CA.</td></tr>

<tr><td width="40" colspan="2"></td><td><textarea name="q11" rows="3" cols="50" wrap="virtual" ></textarea>


<tr><td colspan="3"></td></tr>
<tr><td colspan="3"><p><b>Questions 12-13 Other</b></td></tr>


<tr><td colspan="3"></td></tr>
<tr><td colspan="3"><p><b>Question 12.</b> Are you working within UW Medicine or receiving computing support from UW&nbsp;Medicine&nbsp;ITS?</td></tr>

<tr><td width="20"></td><td align="right"><p><input type="radio" name="q12" value="Yes"></td>
<td><p>Yes</td></tr>

<tr><td width="20"></td><td align="right"><p><input type="radio" name="q12" value="No"></td>
<td><p>No</td></tr>


<tr><td colspan="3"></td></tr>
<tr><td colspan="3"><p><b>Question 13.</b> Please enter any comments or additional information you'd like to send along with this registration.</td></tr>

<tr><td width="40" colspan="2"></td><td><textarea name="q13" rows="5" cols="50" wrap="virtual" ></textarea>
</td></tr>


<tr><td colspan="3"></td></tr>
<tr class=code>
 <td colspan="3" >
  &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;
  <!-- <input type="submit" value="Submit" > -->
  <input type="button" value="Submit" onclick="doform();" >
  </td>
</tr>
<br>

</table>

</form>

# end of the form
