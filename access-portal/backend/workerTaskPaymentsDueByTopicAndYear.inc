<?php
print '<h4>Worker task payments due by topic and year</h4>';

print '<p>Note that a single task may be assigned to multiple topics so a few tasks may be double-counted below. Also, the list below does not include all tasks.</p>';
# Initialize values to zero
$groupingString = '';
$columnsString = '';
foreach($topics as $topic) {
  $groupingString .= 'topic REGEXP "'.$topic.'",';
  $columnsString .= 'topic REGEXP "'.$topic.'" as `topic:'.$topic.'`,';
  $totalPaymentForTopic[$topic] = 0; 
  foreach ($yearList as $year) {
    $paymentForTopicAndYear[$topic][$year] = 0;
  }
}  

foreach($yearList as $year) {
  $totalPaymentForYear[$year] = 0;
}

$grandTotal = 0;

# Get task payments

$taskSelectQuery = "select $columnsString year(completion_date) as year, coalesce(sum(payment), 0) as payment from tasks where worker='$worker' group by $groupingString year(completion_date);";
$taskSelectResult = $mysqli -> query($taskSelectQuery);
for ($i = 0; $i < $taskSelectResult -> num_rows; $i++) {
  $row = $taskSelectResult -> fetch_assoc();
  foreach ($topics as $topic) {
    if ($row['topic:' . $topic] == 1) {
      $paymentForTopicAndYear[$topic][$row['year']] += $row['payment'];
      $totalPaymentForTopic[$topic] += $row['payment'];
    }
  }
  $totalPaymentForYear[$row['year']] += $row['payment'];
  $grandTotal += $row['payment'];
}

function cmpTopics($firstTopic, $secondTopic) {
  global $totalPaymentForTopic;
  if (intval($totalPaymentForTopic[$firstTopic]) == intval($totalPaymentForTopic[$secondTopic])) {
      return 0;
  }
  return (intval($totalPaymentForTopic[$firstTopic]) > intval($totalPaymentForTopic[$secondTopic])) ? -1 : 1;
}

usort($topics,"cmpTopics");

# Print table
print '<table border="1">';
print '<tr><th>Topic</th>';
foreach (array_reverse($yearList) as $year) {
  print "<th>Total in year $year</th>";
}
print '<th>Total across years</th></tr>';

foreach($topics as $topic) {
  if ($totalPaymentForTopic[$topic] > 0) {
    print "<tr><td>$topic</td>";
    foreach (array_reverse($yearList) as $year) {
      print '<td align="right">'.sprintf('%.2f',$paymentForTopicAndYear[$topic][$year]).'</td>';
    }
    print '<td align="right">'.sprintf('%.2f',$totalPaymentForTopic[$topic]).'</td>';
    print '</tr>';
  }
}

print "<tr><td><strong>Total</td>";
foreach (array_reverse($yearList) as $year) {
  print '<td align="right"><strong>'.sprintf('%.2f',$totalPaymentForYear[$year]).'</strong></td>';
}
print '<td align="right"><strong>'.sprintf('%.2f',$grandTotal).'</strong></td>';
print '</tr>';
print '</table>';
?>