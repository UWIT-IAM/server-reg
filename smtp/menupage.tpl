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


# smtp exception application menupage
#

<!-- page -->

<table border=0 cellpadding=0 cellspacing=0>
<tr>
<td align=left valign=top>
 <!-- server menu -->
<table class=nav>


     <tr><td colspan=2 nowrap class=msg>SMTP Exception Application</td></tr>

     <tr><td></td><td nowrap>
      <a href={ROOT}/?req=ft>Apply for a from/to exception</a></td></tr>

     <tr><td></td><td nowrap>
      <a href={ROOT}/?req=dns>Apply for a DNS exception</a></td></tr>

<!--
     <tr><td></td><td nowrap>
      <a href={ROOT}/?req=help>Help</a></td></tr>
 -->

   <tr><td colspan=2 nowrap class=msg>Related pages</td></tr>

     <tr><td></td><td nowrap>
      <a href=https://server-reg.cac.washington.edu/pbc/>Weblogin registration</a></td></tr>

     <tr><td></td><td nowrap>
      <a href=https://server-reg.cac.washington.edu/groups/>Groups registration</a></td></tr>

     <tr><td></td><td nowrap>
      <a href=http://www.washington.edu>UW home</a></td></tr>

     <tr><td></td><td nowrap>
      <a href=http://www.washington.edu/computing/eds/>
           EDS pages</a></td></tr>

</table>

</td>
<td width=20></td>

<!-- content -->

<td valign=top>



<!-- BDB: home -->

<h3>Request for exception to UW authenticated SMTP</h3>
<p>

In order to reduce the amount of 'spam' email sent via the UW email
service, the UW mail sending (SMTP) service (smtp.washington.edu) requires
connections to it be authenticated (using a UW NetID and password) and
"secured" (e.g., using SSL or STARTLS). Most "desktop" email client
software is able to easily meet these requirements.

<p>

However, there will be circumstances under which email needs to be 
submitted via smtp.washington.edu and the sending software or device 
cannot use authentication.  In such cases it is necessary to request a 
special exception to the authentication requirement.  Such exceptions will 
be granted on a temporary basis and subject to periodic (we expect every 6 
months) review.

<p>

Two kinds of exceptions may be requested.
 From/To address exceptions are recommended whenever possible.  An
exception by system name leaves more vulnerability to outbound spam if the system
is compromised so should be used sparingly.
<p>
<ol>
<li>An exception based on the "From/To" pair of email addresses that
allows messages from a particular address to be delivered to a particular
address regardless of the source system. This would be particularly useful
in the case of "reporting" software that is distributed to multiple
computers (or UPS systems, or printers, etc.).
<p>
<p class=indent>
<form action="{ROOT}/" method=get>
<input type=hidden name=req value=ft>
<input type=submit name=sub value="Apply for a from/to exception">
</form>
<p>



<p>
<li>An exception based on the name of the excepted system as registered in
DNS. This would be necessary in the case of a system sending messages to 
many different addresses.

<p>
<p class=indent>
<form action="{ROOT}/" method=get>
<input type=hidden name=req value=dns>
<input type=submit name=sub value="Apply for a DNS exception">
</form>
<p>

</ol>
<p>

<!-- EDB: home -->


<!-- BDB: thanks_ft -->

<h3>Application received</h3>
<p>
 Thanks for submitting your request.  It will be forwarded by 
 email to UWIT staff.  No action will be taken until we get a 
 message from "{TO}" authorizing the exception.

<p>
After review, you'll get an email message to let you know when the 
   exceptions have been set up, or if there is some problem.  You should 
   get a response within three working days.

<!-- EDB: thanks_ft -->


<!-- BDB: thanks_dns -->

<h3>Application received</h3>
<p>

Thanks for submitting your request.  It will be forwarded by 
   email to UWIT staff.  
<p>
A notice has been sent to "{DEAN}" and they will be 
   given two working days to raise any concerns.  No response is needed
   for them to give approval.
<p>
After review, you'll get an email message to let you know when the 
   exceptions have been set up, or if there is some problem.  You 
   should get a response within five working days.

<!-- EDB: thanks_dns -->


</table>

