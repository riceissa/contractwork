<?php

print '<h4 id="topicImpact">Topic impact: Wikipedia</h4>';

$paidViewsQuery = "select sum(viewcount) as totalViews from wikipediaviews.viewcountsbymonth where (pagename,language) in (select distinct task_receptacle,language from tasks where $topicRegexMatcher and task_type in ('Wikipedia page creation','Wikipedia page translation')) and drilldown in ('desktop','mobile-web','mobile-app');";
 
$paidViewsResult = $mysqli -> query($paidViewsQuery);

$paidViewCount = 0;
if ($paidViewsResult -> num_rows > 0) {
  $row = $paidViewsResult -> fetch_assoc();
  $paidViewCount = $row['totalViews'];
}

$paidCreatedOrEditedPageViewsQuery = "select sum(viewcount) as totalViews from wikipediaviews.viewcountsbymonth where (pagename,language) in (select distinct task_receptacle,language from tasks where $topicRegexMatcher and task_type in ('Wikipedia page creation','Wikipedia page translation','Wikipedia page update')) and drilldown in ('desktop', 'mobile-web', 'mobile-app');";

$paidCreatedOrEditedPageViewsResult = $mysqli -> query($paidCreatedOrEditedPageViewsQuery);

$paidCreatedOrEditedPageViewCount = 0;
if ($paidCreatedOrEditedPageViewsResult -> num_rows > 0) {
  $row = $paidCreatedOrEditedPageViewsResult -> fetch_assoc();
  $paidCreatedOrEditedPageViewCount = $row['totalViews'];
}


$costPerThousandViews = 0;
$blendedCostPerThousandViews = 0;
if ($paidViewCount > 0) {
  $costPerThousandViews = 1000 * ($totalPaymentForTaskType['Wikipedia page creation'] + $totalPaymentForTaskType['Wikipedia page translation'])/$paidViewCount;
  $blendedCostPerThousandViews = 1000 * $totalTaskPaymentAcrossYears/$paidViewCount;
}
$costPerThousandViewsCreatedOrEdited = 0;
$blenddCostPerThousandViewsCreatedOrEdited = 0;
if ($paidCreatedOrEditedPageViewCount > 0) {
  $costPerThousandViewsCreatedOrEdited = 1000 * ($totalPaymentForTaskType['Wikipedia page creation'] + $totalPaymentForTaskType['Wikipedia page translation'] + $totalPaymentForTaskType['Wikipedia page update'])/$paidCreatedOrEditedPageViewCount;
  $blendedCostPerThousandViewsCreatedOrEdited = 1000 * $totalTaskPaymentAcrossYears/$paidCreatedOrEditedPageViewCount;
}

print "\n";
print '<table id="myTableTopicImpact" class="tablesorter">'."\n";
print "<thead>\n";
print "<tr>\n    <th>Item</th>\n    <th>Value</th>\n</tr>\n";
print "</thead>\n<tbody>\n";
print "<tr>\n    <td>Total pageviews of Wikipedia pages created as contract work</td>\n    ".'<td align="right">'.sprintf('%.2f',$paidViewCount)."</td>\n</tr>\n";
print "<tr>\n    <td>Total pageviews of Wikipedia pages created or edited (not necessarily significantly) as contract work</td>\n    ".'<td align="right">'.sprintf('%.2f',$paidCreatedOrEditedPageViewCount)."</td>\n</tr>\n";
print "<tr>\n    <td>Cost per thousand views of pages created (including only explicit costs for page creation)</td>\n    ".'<td align="right">'.sprintf('%.2f',$costPerThousandViews)."</td>\n</tr>\n";
print "<tr>\n    <td>Cost per thousand views of pages created (including all task pay)</td>\n".'    <td align="right">'.sprintf('%.2f',$blendedCostPerThousandViews)."</td>\n</tr>\n";
print "<tr>\n    <td>Cost per thousand views of pages created or edited (including only explicit costs for page creation and editing)</td>\n".'    <td align="right">'.sprintf('%.2f',$costPerThousandViewsCreatedOrEdited)."</td>\n</tr>\n";
print "<tr>\n    <td>Cost per thousand views of pages created or edited (including pay for everything)</td>\n".'    <td align="right">'.sprintf('%.2f',$blendedCostPerThousandViewsCreatedOrEdited)."</td>\n</tr>\n";
print "</tbody>\n</table>\n";
?>