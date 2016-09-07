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

# Pubcookie authorization page
#

<html>
<head>
<link rel=stylesheet type="text/css" href="/server-reg.css">

<title>{TITLE}</title>

{HEADTXT}

<!-- BDB: adminjs -->

# admin page menu functions 

<script language=javascript>
function dolist(l,r)
{
   if (l!="") document.list.list.value=l;
   if (r!="") document.list.req.value=r;
   document.list.submit();
}

function checkkey(l,r,e)
{
  var code = 0;
  if (window.event) code = window.event.keyCode;
  else if (e) code = e.which;
  if (code==13) dolist(l,r);
}

</script>



<!-- EDB: adminjs -->

</head>

<body {BODYTXT}>

<!-- header -->


<table border=0>
<tr>
  <td><img src=/UWlogo.gif valign=bottom></td>
  <td width=20></td>
  <td valign=bottom class=head>Weblogin Server Registration</td>
</tr>
</table>




<!-- crumbs -->

<table class=crumbtop cellpadding=0 cellspacing=0 width=100%>
<tr><td colspan=99 height=0 width=100%></td></tr>
<tr align=left>
 <td><table cellpadding=0 cellspacing=0>
  <tr>
  <td class=crumb valign=bottom nowrap>
   <!-- BDB: crumb -->
    <!-- BDB: sep -->
     |
    <!-- EDB: sep -->
   <a href={ROOT}/{CRUMBLINK}>{CRUMBNAME}</a>
   <!-- EDB: crumb -->
  </td>
  <td width=5></td>
  <!-- BDB: crumbhelp -->
    <td class=crumbh valign=bottom nowrap>
     <a href={ROOT}/{CRUMBLINK}>{CRUMBNAME}</a>
    </td>
  <!-- EDB: crumbhelp -->
  <!-- BDB: preapp -->
    <td class=crumbh valign=bottom nowrap>
     You are pre-approved!
    </td>
  <!-- EDB: preapp -->
 </tr></table></td>


 <!-- BDB: admin -->
   <td width=10></td>
     <form name=list method=post action={ROOT}/admin/>
       <input type=hidden name=list value={LIST}>
       <input type=hidden name=req value=>
   <td align=right><table cellpadding=0 cellspacing=0 >
   <tr>
#   <td class=crumb valign=bottom nowrap>
#      <a class=bar href="{ROOT}/admin/?req=users">Users</a> 
#   </td>
   <td class=crumb valign=bottom nowrap>
      <a class=bar href="{ROOT}/admin/">Pending forms</a> 
   </td>
   <td class=crumb valign=bottom nowrap>
      <a class=bar href="{ROOT}/admin/?req=app">Approved forms</a> 
   </td>
   <td class=crumb valign=bottom nowrap>
      <a class=bar href="{ROOT}/admin/?req=deny">Denied forms</a> 
   </td>
#   <td class=crumb valign=bottom nowrap>
#      <a class=bar href="javascript:dolist('','review');">Check host</a> 
#       <input class=plain type=text value="{DN}" size=25 maxwidth=60 name=dn
#           onKeyPress="checkkey('','test',event);">
#   </td>
   </tr></table></td>
      </form>
 <!-- EDB: admin -->


 <!-- BDB: not_admin -->
   <td align=right><table cellpadding=0 cellspacing=0 >
   <tr>
    <td class=crumb valign=bottom nowrap>
    <a href="{ROOT}/admin/">Administration</a>
    </td>
   </tr></table></td>
 <!-- EDB: not_admin -->
 
</tr>
</table>

      
<!-- content -->
<h3>{PAGETITLE}</h3>
<p>

<div class=toptext>
 {TOPBODY}
</div>
<p class=msg>{MSG}</p>
<p class=err>{EMSG}</p>

# this text for already approved server 
<!-- BDB: already -->
<p class=msg>
Server {CN} is already OK for pubcookie keys.
<!-- EDB: already -->

<!-- BDB: showonr -->
<p class=err >chk_dn says: <tt>{ONRMSG}</tt><p>
<!-- EDB: showonr -->

# this text for approved submission 
<!-- BDB: permitted -->
<p class=msg>
Server {CN} is now authorized for pubcookie keys.
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
Your request for pubcookie keys for server {CN} has been submitted.
<p>
<!-- EDB: submitted -->



# this text for admin detail
<!-- BDB: detail -->
<div class=code>
<pre>
<asis>
{TEXT}
</asis>
</pre>
</div>
<!-- EDB: detail -->


<!-- main text -->
<p>
{DETAIL}
<p>
<p>
{BODY}

<p>
# standard footer
<table width=100% cellpadding=0 cellspacing=0 border=0>
<tr>
<td class=footleft width=48 height=48><img src=/UWseal.gif></td>
<td align=left class=footright>
<a href="http://depts.washington.edu/cac/">Computing
&amp; Communications</a><br/>
help@cac.washington.edu<br/>
Modified: September 20, 2005
</td>
<td align=right class=footright valign=top>
A service provided by<br>
<img src=/CnC.gif>
</td>
</tr>
</table>



</html>
