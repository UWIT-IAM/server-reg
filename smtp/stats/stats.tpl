
# This is actually parsed by lib/mkstats, run from cron,
# and stored in the stats directory


<h3>UW Services CA</h3>

<p align=center>
<img class=policy src=stats/sum.png>
</p>
<p>
<table cellpadding=1>
<tr><td colspan=2>Total certificates in use:</td><td align=right>{NCRT}</td></tr>
<tr><td width=20></td><td><i>by domain</i></td></tr>
<!-- BDB: dom -->
<tr><td width=20></td>
    <td><tt>{DNAME}&nbsp;</tt></td><td align=right>{DCNT}</td></tr>
<!-- EDB: dom -->
</table>
<p>

