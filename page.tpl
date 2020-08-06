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
# server-reg common page 
#

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en">
<head>
<title>{TITLE}</title>
 <!-- <link rel="stylesheet" type="text/css" href="/css/iam-dojo-2.css"/> -->
 <link rel="stylesheet" type="text/css" href="/server-reg/server-reg.css"/>


{JS_LIST}

</head>


<body {BODYTXT} >

<div id="thepage" style=" overflow:auto;">


<h1 class="hidden">SERVER REGISTRATION</h1>
<!--
<div id="topbanner" >
<span class="topleft"><a href="https://wiki.cac.washington.edu/x/lR4">IDENTITY AND ACCESS MANAGEMENT</a></span>
<ul class="topright">
 <li>Logged in: {REMOTE_USER} </li>
 <li>|</li>
 <li><a href="javascript:document.location='/logout/';">logout</a></li>
 <li>|</li>
 <li><a href="https://wiki.cac.washington.edu/x/gjyoAQ" target="sphelp">HELP</a></li>
</ul>
</div>

<div id="banner" style="padding: 0px; border: 0px">
    <span id="title"><a tabindex=-1 href="javascript:showHomePage()">SERVER REGISTRATION</a></span>
    <span id="banner_notice">
    </span>
    <ul id="ident">
        <li></li>
    </ul>
</div>

-->



# crumb line

<table class=crumbtop cellpadding=0 cellspacing=0 width=100%>
<tr><td colspan=99 height=0 width=100%></td></tr>
<tr align="left">
<td width="2"></td>

# crumbs
 <td><table cellpadding=0 cellspacing=0>

  <tr>
  <td class=crumb valign=bottom nowrap>
   <!-- BDB: crumbsr -->
   <a href="/server-reg/">Server registration home</a>
   <!-- EDB: crumbsr -->
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

 <!-- BDB: crumbadmin -->
   <td align=right><table cellpadding=0 cellspacing=0 >
   <tr>
    <td class=crumb valign=bottom nowrap>
    <a href="{ROOT}/admin/">Administration</a>
    </td>
   </tr></table></td>
 <!-- EDB: crumbadmin -->

</tr>
</table>

<p>


# page (menu + content)

<table border=0 cellpadding=1 cellspacing=0 style="margin-left:10px;">
<tr>

<!-- BDB: menu -->
<td align=left valign=top>
{MENU}
</td>
<td width=20></td>
<!-- EDB: menu -->


<td valign=top>

<span class=title>{PAGETITLE}</span>

# generic messages
<span lass=msg>{MSG}</span>
<span class=err>{EMSG}</span>

#
# actual page text
#

<div>
{BODY}

# a go back message
<!-- BDB: goback -->
<span class=indent>
<form action="/" method=get>
<input type=button value="Return to the form" onclick="back()">
</form>
</span>
<!-- EDB: goback -->

</div>

</td>
</tr>
</table>

<p>
</div>

<!--
<div id="footer"  style="position:absolute;bottom:0px">
    <span id="footlogo"><a href="http:/www.uw.edu/" title="University of Washington">
       UNIVERSITY OF WASHINGTON
       </a></span>
    <span id="footcenter">
    <ul id="navigation">
        <li><a href="mailto:help@uw.edu">CONTACT US</a></li>
        <li>|</li>
        <li><a href="http://www.washington.edu/online/privacy/" target="sppriv">PRIVACY</a></li>
        <li>|</li>
        <li><a href="http://www.washington.edu/online/terms/" target="spterms">TERMS</a></li>
    </ul>
    </span>
    <span id="footright">
    <ul id="navigation">
        <li><a href="javascript:iam_showTheDialog('helpAbout');">Server Registry 2.2</a></li>
    </ul>
    </span>
</div>

-->




</html>

