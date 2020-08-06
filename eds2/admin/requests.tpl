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

# EDS admin request list

<table cellpadding=0 cellspacing=0 border=0>
<tr>
<td align=left valign=top>
<h3>EDS Applications</h3>
<p>
<!-- BDB: allapps -->
This is table shows all EDS applications.  You can 
choose to see only those that require administrative action.
<p>
<ul>
<li><a href="?req=apps&select=active">Show only active applications<a/>
</ul>
<!-- EDB: allapps -->

<!-- BDB: actapps -->
This is table shows EDS applications that need administrative action.  You can 
choose to see all applications.
<p>
<ul>
<li><a href="?req=apps&select=all">Show all applications<a/>
</ul>
<!-- EDB: actapps -->
<p>
</td>
<td width=100></td>
<td align=right valign=top border=1>
<table class=keynote style="border-width:1px">
<tr><td><img src=/server-reg/icons/whiteball.gif> </td><td nowrap>No request, no permission</td></tr>
<tr><td><img src=/server-reg/icons/greenball.gif> </td><td nowrap>Requested, permission granted</td></tr>
<tr><td><img src=/server-reg/icons/orangeball.gif> </td><td nowrap>Requested, no permission</td></tr>
<tr><td><img src=/server-reg/icons/redball.gif></td> <td nowrap>No request, permission granted</td></tr>
</table>
</td>
</tr>
</table>
<p>


<table cellpadding=3 cellspacing=3 border=1>

# header
<tr>
  <td></td>
  <td></td>
  <td colspan=4>PDS</td>
  <td colspan=2>GDS</td>
  <td colspan=2></td>
  <td colspan=2></td>
</tr>
<tr>
  <td></td>
  <td>Application</td>
  <td>Bas</td>
  <td>Std</td>
  <td>Emp</td>
  <td>Alm</td>
  <td>Bas</td>
  <td>Crs</td>
# <td>Auth group</td>
#  <td>Group email</td>
</tr>

# forms
<!-- BDB: cn -->
<tr>
  <td class=menu><a href={ROOT}/admin/?req=detail&cn={CN}>Show</a></td>
  <td class=code>{CN}</td>
  <!-- BDB: auth -->
  <td><img src="{auth_img}"></td>
  <!-- EDB: auth -->
# <td>{app_grp}</td>
#  <td>{app_email}</td>
</tr>
<!-- EDB: cn -->
</table>

<!-- BDB: none -->
No applications in this list.
<!-- EDB: none -->
