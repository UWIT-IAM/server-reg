#  ========================================================================
#  Copyright (c) 2005 The University of Washington
# 
# $HeadURL: https://svn.cac.washington.edu/svn/ateam/mailscripts/trunk/server-reg/dns_form.tpl $
# $Id: dns_form.tpl 747 2012-10-26 00:37:06Z hubert@u.washington.edu $
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

# SMTP dns exception request template

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
  doc.request.req.value="dnssub";
  doc.request.submit();
}

</script>


<form action="{ROOT}/" method="post" name="request">
  <input type=hidden name=req value="dnssub">
  <input type=hidden name=xseq value={XSEQ}>



The owner of a system (as registered in DNS) can request an exception
for that system so that it can send mail via smtp.uw.edu WITHOUT
authentication, IF that system:

<p>
<ol>
 <li> is a "server" (not a "workstation" system, e.g., not running
"productivity" apps), the implication is that it should be more tightly
controlled, monitored and supported than a typical "user workstation".

 <li> is in fact, well supported, updated, monitored and controlled.

 <li> is at a static IP address with both forward and reverse DNS correctly
defined.

 <li> needs to send mail, and the software actually sending the mail doesn't
support authenticated SMTP.

 <li> you have obtained the consent of your Dean, Director or VP for this
exception (an email will be sent reporting that the consent has
been granted).
</ol>
<p>


<hr>
<table cellpadding=3>
<tr><td>Does your system meet all these requirements?</td><td><input type="checkbox" name="meets" value="yes">yes</td></tr>



<tr><td>DNS name of the system to be excepted:</td><td>
     <input type="text" name="dns" value="{DNSNAME}" size=40 </td></tr>

<!-- version with the button
<tr><td>DNS name of the system to be excepted:</td><td>
     <input type="text" name="dns" value="{DNSNAME}" size=40 onKeyPress="checkkey('','',event);"></td></tr>
<tr><td></td><td><input type="button" value="Test DNS ownership" onclick="cntest();" ></td></tr>
 -->

<tr><td>Email address of consenting Dean, Director or VP: </td><td>
     <input type="text" name="dean" value="{DEANADDR}" size=40></td></tr>


</table>
<p>

  <input type="button" value="Submit the request" onclick="doform();" >

</form>

# end of the form
