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

#
# eds registration homepage
#

Welcome to the UW Enterprise Directory Service (EDS) client application
registration website, where UW administrators can register new client
applications for use with the UW Person Directory Service (PDS), Person Web Service (PWS) and Groups
Directory Service (GDS).  To register a client application, you'll need some
information at hand about the application:

<p>
<ul>
<li>
Its DNS name.  This is the CN of the certificate you will
be using to authenticate to the directory service.  
You must use a UW Certificate Authority (UWCA)
certificate to authenticate to the directory service.

<p>
<li>
The technical and administrative contact people for the application.
<p>
<li>
The directory entries and attributes it needs access to.
<p>
<li>
Its purpose, client technology, and security procedures.
</ul>

<p>
This information helps the iam-support team ensure that your
application's access is compliant with policy, and helps the team better
understand and support directory customers.
<p>
<p>
<p class=indent>
<form action="{ROOT}/" method=post>
<input type=hidden name=req value=req_1>
<input type=submit name=sub value="Register a client application">
</form>
<p>
</ul>

<p>

</ul>


