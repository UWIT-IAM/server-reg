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

#
# pubcookie registration homepage
#

Welcome to the UW Weblogin Server Registration website, where 
UW web server administrators can register new servers for use
with the University of Washington's
<a href="http://www.washington.edu/computing/pubcookie/">
weblogin service</a>.

<p>
At this site you may:
<p>
<ul>
<li>Register a new service to use weblogin. 
<p>
<ul>
<li>Registration is based on the domain name of your web server and
must match the Common Name (CN) attribute of your server's 
SSL certificate.  
<p>
<li>Registration requires that your UW NetID be listed in UW DNS
as an owner of your server's domain name.
<p>
<p class=indent>
<form action="{ROOT}/" method=get>
<input type=hidden name=req value=form>
<input type=submit name=sub value="Register a server">
</form>
<p>
</ul>

<p>

</ul>


