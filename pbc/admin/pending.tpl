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

# pending request list

<script language=javascript>
function doaction(r, c)
{
   document.act.req.value=r;
   document.act.cn.value=c;
   document.act.submit();
}
</script>

<form name=act method=post action={ROOT}/admin/>
<input type=hidden name=cn value=yyy>
<input type=hidden name=req value=xxx>
<input type=hidden name=xseq value={XSEQ}>
</form>


<h3>Pending forms</h3>
<p>

<for
<ul>
<!-- BDB: cn -->
<li>
  {CN} from {ID}
  <br>
  <ul class=nav>
  <li><a href="javascript:doaction('ok', '{CN}')">Approve this request</a>
  &nbsp;
  <li><a href="javascript:doaction('no', '{CN}')">Deny this request</a>
  &nbsp;
  <li><a href="javascript:doaction('detail', '{CN}')">Show the form</a>
  </ul>
<p>
<!-- EDB: cn -->
</ul>

<!-- BDB: none -->
No pending requests.
<!-- EDB: none -->
