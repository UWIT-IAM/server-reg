#  ========================================================================
#  Copyright (c) 2006-2007 The University of Washington
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

# EDS registration auth group 

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
  doc.request.req.value="rag_3";
  doc.request.submit();
}

</script>


<form action="{ROOT}/" method="post" name="request">
  <input type=hidden name=req value=req_3>
  <input type=hidden name=xsite value={XSITE}>
  <input type=hidden name=xtime value={XTIME}>
  <input type=hidden name=cn value={CN}>


<b>Application DNS name: <tt>{CN}</tt></b>

<table width="100%" border="0" cellpadding="4">


<tr><td colspan="99"></td></tr>
<tr><td colspan="99"><p><b>Application authorization group:</b>
   Please enter the name of the authorization group for this application.  
 </td></tr>
  
<tr><td width="40" colspan="2"></td><td>
   <input type="text" name="ag" size="40" value="{AG}">
     </td></tr>
<p>

<tr><td colspan="99"></td></tr>
<tr class=code>
 <td colspan="99" >
  &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;
  <!-- <input type="submit" value="Submit" > -->
  <input type="button" value="Submit" onclick="doform();" >
  </td>
</tr>
<br>

</table>

</form>

# end of the DNS form
