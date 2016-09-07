#  ========================================================================
#  Copyright (c) 2005 The University of Washington
# 
# $HeadURL: https://svn.cac.washington.edu/svn/ateam/mailscripts/trunk/server-reg/ft_form.tpl $
# $Id: ft_form.tpl 747 2012-10-26 00:37:06Z hubert@u.washington.edu $
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

# SMTP exception request, from/to form

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


function doform()
{
  doc.request.req.value="ftsub";
  doc.request.submit();
}

</script>


<form action="{ROOT}/" method="post" name="request">
  <input type=hidden name=req value="ftsub">
  <input type=hidden name=xseq value={XSEQ}>



You are requesting an exception based on the "Envelope From/To" pair of
email addresses. 
This type of exception allows messages from a particular address to be delivered
to a particular address regardless of the source system. The destination
address should NOT be a "personal" UW NetID but a "departmental" (e.g.,
supplemental) ID or email (e.g., "Mailman") list. Mail will be sent to
the To address for an opportunity to review and weigh in.
No response is needed for them to give approval.

<p>
The From/To address pairs referred to here are actually the so called
envelope From and envelope To addresses. That is, the address given in the
SMTP MAIL FROM command, and the address given in the SMTP RCPT TO command.
<p>
<table cellpadding=10 border=0>
<tr>
  <td align=left>Envelope From address:</td><td><input name="fromaddr" value="{FROMADDR}" size=40></td>
</tr>
<tr>
  <td align=left>Envelope To address:</td><td><input name="toaddr" value="{TOADDR}" size=40></td>
</table>
<p>

If you have a large list of From/To pairs of addresses, please send email 
to help@uw.edu.
<p>


  <input type="button" value="Submit the request" onclick="doform();" >

</form>

# end of the form
