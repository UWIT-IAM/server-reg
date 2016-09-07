
# auto form submission

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


