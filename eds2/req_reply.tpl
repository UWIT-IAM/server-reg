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

# request reply pages

<!-- BDB: showonr -->
<p class=err >chk_dn says: <tt>{ONRMSG}</tt><p>
<!-- EDB: showonr -->
   
# this text for approved submission 
<!-- BDB: permitted -->
<p class=msg>
Server {CN} is now authorized for EDS access.
<p>    
<!-- EDB: permitted -->

# this text for denied submission
<!-- BDB: denied -->
<p class=msg>
Authorization for server {CN} denied.
<p>
<!-- EDB: denied -->

# this text for other submission 
<!-- BDB: submitted -->
<p class=msg>
Your request for EDS access for server {CN} has been submitted.
<p> 
<!-- EDB: submitted -->
 
# this text for auth group ok
<!-- BDB: agok -->
<p class=msg>
EDS application "{CN}" is now owned by the group: {AG}
<p> 
<!-- EDB: agok -->
 
# this text for copy page not found
<!-- BDB: noccn -->
<p class=msg>
Could not find the form to copy.
<p> 
<!-- EDB: noccn -->
 



<!-- BDB: nodns -->
There is <b>no</b> DNS entry for {CN}. 
<p>
We cannot accept applications for non-existant domains.
<p>

<!-- EDB: nodns -->

<!-- BDB: ok -->
You are already the owner of {CN} and may register 
this host.
<p>
You may go back and submit the form.
<!-- EDB: ok -->

<!-- BDB: notowner -->

You are <b>not</b> registered as the owner of {CN}.

<p>
You cannot request authorization for this host until
you are known to Network Operations as the owner.

<p>
See <a href="https://wiki.cac.washington.edu/x/eRcrAQ">Managing DNS Names
for Infrastructure Services Access</a> for more information on DNS ownership.

<p>

<!-- EDB: notowner -->


<!-- BDB: goback -->
<p>
<hr>

<p class=indent>
<form action="{ROOT}/{BACK}" method=get>
<input type=button value="Return to the form" onclick="submit();"></a>
<p>
<!-- EDB: goback -->
