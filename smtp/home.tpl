#  ========================================================================
#  Copyright (c) 2005 The University of Washington
# 
# $HeadURL: https://svn.cac.washington.edu/svn/ateam/mailscripts/trunk/server-reg/home.tpl $
# $Id: home.tpl 747 2012-10-26 00:37:06Z hubert@u.washington.edu $
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

#
# smtp exception application home
#

<p>
<h3>Request for exception to UW authenticated SMTP</h3>
<p>

To reduce the amount of 'spam' email sent via the UW email
service, the UW mail sending (SMTP) service (smtp.uw.edu)
requires authenticated (using a UW NetID and
password) and "secured" (e.g., using SSL or STARTLS) connections.

<p>

There may be circumstances when email needs to
be submitted via smtp.uw.edu and the sending software or
device cannot use authentication. In such cases it is necessary
to request an exception to the authentication requirement.
Such exceptions will be granted on a temporary basis and subject
to annual review.

<p>

Two kinds of exceptions may be requested. From/To address
exceptions are recommended whenever possible. An exception by
system name leaves more vulnerability to outbound spam if the
system is compromised so should be used sparingly.
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

