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

#  dn help for smtp pages


<!-- BDB: nodns -->
There is <b>no</b> DNS entry for {CN}. 
<p>
We cannot accept applications for non-existant domains.
<p>

<!-- EDB: nodns -->

<!-- BDB: ok -->
You are already the owner of {CN} and may ask for an exception for
this host.
<p>
You may go back and submit the form.
<!-- EDB: ok -->

<!-- BDB: notok -->

You are <b>not</b> registered as a responsible individual for {CN}.

<p>
You cannot request an exception for this host until
you are so known to Network Operations.


<p>

<!-- BDB: host -->

<hr>
<p>

  <!-- BDB: noowner -->
  
  If you are the responsible individual for the host 
   <p class=indent>
   <tt>{SDN}</tt>
   <p>
  send the following message to 
  <a href=mailto:netops@uw.edu>netops@uw.edu</a>
  <p>
  with a subject of: "<tt>UW&nbsp;NetID&nbsp;update&nbsp;for&nbsp;{SDN}</tt>"

<p>
<div class=indent>
<tt>

As an administrator of the host {SDN},
I would like to add my UW NetID to its DNS record to facilitate
automated management operations.
<p>
UW NetID: {RUID}
<br>
Host    : {SDN}
<p>

Thank you,
<p>
<i>Your-name-here</i>
</tt>
</div>
  

  <!-- EDB: noowner -->
  
  
  <!-- BDB: owners -->
  
  Our records show the following people
  are responsible for the host
   <p class=indent>
   <tt>{SDN}</tt>
   <p>
  <p>
  <ul>
   <!-- BDB: id -->
     <li>{ID}
   <!-- EDB: id -->
  </ul>
  <p>
  
  If you would like to request an exception for this host
  send the following message to 
  <a href=mailto:netops@uw.edu>netops@uw.edu </a>
  <p>
  with a subject of: "<tt>UW&nbsp;NetID&nbsp;update&nbsp;for&nbsp;{SDN}</tt>"

<p>
<div class=indent>
<tt>
As an administrator of the host {SDN},
I would like to add my UW NetID to its DNS record to facilitate
automated management operations.
<p>
UW NetID: {RUID}
<br>
Host    : {SDN}
<p>
Thank you,
<p>
<i>Your-name-here</i>

</tt>
</div>
  Otherwise, you may ask
    <!-- BDB: one -->
     the listed responsible person
    <!-- EDB: one -->
    <!-- BDB: many -->
     one of the listed responsible people
    <!-- EDB: many -->
  to make the exception request for you.
<p>
  
  <!-- EDB: owners -->

<!-- EDB: host -->


<!-- BDB: domain -->

<hr>
<p>

  <!-- BDB: noowner -->
  
  If you are responsible for the entire domain 
   <p class=indent>
   <tt>{SDN}</tt>
   <p>
  and would like to be able to request exceptions for 
  any of host on that domain
  send the following message to 
  <a href=mailto:netops@uw.edu>netops@uw.edu</a>
  <p>
  with a subject of: "<tt>UW&nbsp;NetID&nbsp;update&nbsp;for&nbsp;{SDN}</tt>"

<p>

<div class=indent>
<tt>
As an administrator of the domain/subnet {SDN},
I would like to add my UW NetID to its DNS record to facilitate
automated management operations.
<p>
UW NetID: {RUID}
<br>
Domain  : {SDN}
<p>
Thank you,
<p>
<i>Your-name-here</i>
</tt>
</div>
  

  <!-- EDB: noowner -->
  
  
  <!-- BDB: owners -->
  
  Our records show the following people
  as responsible for the domain
   <p class=indent>
   <tt>{SDN}</tt>
   <p>
  <p>
  <ul>
   <!-- BDB: id -->
     <li>{ID}
   <!-- EDB: id -->
  </ul>
  <p>
  
  If you are responsible for all hosts on the domain and
  would like to request exceptions for any of those hosts
  send the following message to 
  <a href=mailto:netops@uw.edu>netops@uw.edu</a>
  <p>
  with a subject of: "<tt>UW&nbsp;NetID&nbsp;update&nbsp;for&nbsp;{SDN}</tt>"

<p>
<div class=indent>
<tt>
As an administrator of the domain/subnet {SDN},
I would like to add my UW NetID to its DNS record to facilitate
automated management operations.
<p>
UW NetID: {RUID}
<br>
Domain  : {SDN}
<p>
Thank you,
<p>
<i>Your-name-here</i>

</tt>
</div>
  
  Otherwise, you may ask
    <!-- BDB: one -->
     the listed responsible person
    <!-- EDB: one -->
    <!-- BDB: many -->
     one of the listed responsible people
    <!-- EDB: many -->
  to make the exception request for you.
<p>
  
  <!-- EDB: owners -->

<!-- EDB: domain -->

<!-- EDB: notok -->

<p>
<hr>

<p class=indent>
<form action="/" method=post>
<input type=button value="Return to the form" onclick="history.back();"></a>
<p>
