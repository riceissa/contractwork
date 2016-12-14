<?php
if ($printTables == true) {
  print '<h4>Worker payments due by type and month</h4>';
}

# Initialize values to zero
foreach ($monthList as $month) {
  $taskPayment[$month] = 0;
  $totalTaskPayment = 0;
  $commissionOnTaskPayment[$month] = 0;
  $totalCommissionOnTaskPayment = 0;
  $stipendPayment[$month][0] = 0;
  $totalStipendPayment[0] = 0;
  $stipendPayment[$month][1] = 0;
  $totalStipendPayment[1] = 0;
  $royaltyPayment[$month] = 0;
  $totalRoyaltyPayment = 0;
  $commissionOnRoyaltyPayment[$month] = 0;
  $totalCommissionOnRoyaltyPayment = 0;
  $totalPaymentAwardedAcrossMonths = 0;
  $totalPaymentAcrossMonths = 0;
}  

# Get task payments

$taskSelectQuery = "select year(completion_date) as year,month(completion_date) as month, coalesce(sum(payment), 0) as payment from tasks where worker='$worker' group by year(completion_date), month(completion_date);";
$taskSelectResult = $mysqli -> query($taskSelectQuery);
for ($i=0; $i < $taskSelectResult -> num_rows; $i++) {
  $row = $taskSelectResult -> fetch_assoc();
  $taskPayment[$row['year'] . '-' . sprintf('%02d',$row['month'])] = $row['payment'];
  $totalTaskPayment += $row['payment'];
}

# Get commission on task payments
$commissionOnTaskSelectQuery = "select year(completion_date) as year, month(completion_date) as month, coalesce(sum(payment), 0) as payment from (select commission_receiver, completion_date, (commission * payment) as payment from commissions left join tasks on commissions.earner = tasks.worker where (commission_validity_start is null or tasks.completion_date >= commission_validity_start) and (commission_validity_end is null or tasks.completion_date <= commission_validity_end)) t1 where commission_receiver='$worker' group by year(completion_date), month(completion_date);";
$commissionOnTaskSelectResult = $mysqli -> query($commissionOnTaskSelectQuery);
for ($i = 0; $i < $commissionOnTaskSelectResult -> num_rows; $i++) {
  $row = $commissionOnTaskSelectResult -> fetch_assoc();
  $commissionOnTaskPayment[$row['year'] . '-' . sprintf('%02d',$row['month'])] = $row['payment'];
  $totalCommissionOnTaskPayment += $row['payment'];
}

# Get stipend payments

$stipendSelectQuery = "select year(stipend_end_date) as year, month(stipend_end_date) as month, (stipend_award_date is not null) as awarded, coalesce(sum(payment), 0) as payment from stipends where payee='$worker' group by year(stipend_end_date), month(stipend_end_date), (stipend_award_date is not null);";
$stipendSelectResult = $mysqli -> query($stipendSelectQuery);
for ($i = 0; $i < $stipendSelectResult -> num_rows; $i++) {
  $row = $stipendSelectResult -> fetch_assoc();
  $stipendPayment[$row['year'] . '-' . sprintf('%02d',$row['month'])][$row['awarded']] = $row['payment'];
  $totalStipendPayment[$row['awarded']] += $row['payment'];
}

# Get royalty payments
$royaltySelectQuery = "select year(royalty_end_date) as year, month(royalty_end_date) as month, coalesce(sum(payment), 0) as payment from royalties where payee='$worker' group by year(royalty_end_date), month(royalty_end_date);";
$royaltySelectResult = $mysqli -> query($royaltySelectQuery);
for ($i = 0; $i < $royaltySelectResult -> num_rows; $i++) {
  $row = $royaltySelectResult -> fetch_assoc();
  $royaltyPayment[$row['year'] . '-' . sprintf('%02d',$row['month'])] = $row['payment'];
  $totalRoyaltyPayment += $row['payment'];
}

# Get commission on royalty payments
$commissionOnRoyaltySelectQuery = "select year(royalty_end_date) as year, month(royalty_end_date) as month, coalesce(sum(payment), 0) as payment from (select commission_receiver, royalty_end_date, (commission * payment) as payment from commissions left join royalties on commissions.earner = royalties.payee where (commission_validity_start is null or royalties.royalty_start_date >= commission_validity_start) and (commission_validity_end is null or royalties.royalty_end_date <= commission_validity_end)) t1 where commission_receiver='$worker' group by year(royalty_end_date), month(royalty_end_date);";
$commissionOnRoyaltySelectResult = $mysqli -> query($commissionOnRoyaltySelectQuery);
for ($i = 0; $i < $commissionOnRoyaltySelectResult -> num_rows; $i++) {
  $row = $commissionOnRoyaltySelectResult -> fetch_assoc();
  $commissionOnRoyaltyPayment[$row['year'] . '-' . sprintf('%02d',$row['month'])] = $row['payment'];
  $totalCommissionOnRoyaltyPayment  += $row['payment'];
}

foreach($monthList as $month) {
  $totalPaymentAwarded[$month] = 0;
  $totalPayment[$month] = 0;
  $totalPaymentAwarded[$month] = $taskPayment[$month] + $commissionOnTaskPayment[$month]  + $stipendPayment[$month][1] + $royaltyPayment[$month] + $commissionOnRoyaltyPayment[$month];
  $totalPaymentAwardedAcrossMonths += $totalPaymentAwarded[$month];
  $totalPayment[$month] = $totalPaymentAwarded[$month] + $stipendPayment[$month][0];
  $totalPaymentAcrossMonths += $totalPayment[$month];
}

# Print table
if ($printTables == true) {
  print '<table border="1">';
  print '<tr><th>Month</th>';
  print '<th>Task payments</th>';
  print '<th>Commissions on task payments</th>';
  print '<th>Stipend payments (awarded)</th>';
  print '<th>Royalty payments</th>';
  print '<th>Commissions on royalty payments</th>';
  print '<th>Total payment awarded</th>';
  print '<th>Stipend payments (deferred)</th>';
  print '<th>Total payment</th>';
  print '</tr>';
  
  foreach (array_reverse($monthList) as $month) {
    if ($totalPayment[$month] > 0) {
      print "<tr><td>$month</td>";
      print '<td align="right">'.sprintf('%.2f',$taskPayment[$month]).'</td>';
      print '<td align="right">'.sprintf('%.2f',$commissionOnTaskPayment[$month]).'</td>';
      print '<td align="right">'.sprintf('%.2f',$stipendPayment[$month][1]).'</td>';
      print '<td align="right">'.sprintf('%.2f',$royaltyPayment[$month]).'</td>';
      print '<td align="right">'.sprintf('%.2f',$commissionOnRoyaltyPayment[$month]).'</td>';
      print '<td align="right"><strong>'.sprintf('%.2f',$totalPaymentAwarded[$month]).'</strong></td>';
      print '<td align="right">'.sprintf('%.2f',$stipendPayment[$month][0]).'</td>';
      print '<td align="right"><strong>'.sprintf('%.2f',$totalPayment[$month]).'</strong></td>';
      print '</tr>';
    }
  }
  
  print '<tr><td><strong>Total across months</strong></td>';
  print '<td align="right"><strong>'.sprintf('%.2f',$totalTaskPayment).'</td>';
  print '<td align="right"><strong>'.sprintf('%.2f',$totalCommissionOnTaskPayment).'</td>';
  print '<td align="right"><strong>'.sprintf('%.2f',$totalStipendPayment[1]).'</td>';
  print '<td align="right"><strong>'.sprintf('%.2f',$totalRoyaltyPayment).'</td>';
  print '<td align="right"><strong>'.sprintf('%.2f',$totalCommissionOnRoyaltyPayment).'</td>';
  print '<td align="right"><strong>'.sprintf('%.2f',$totalPaymentAwardedAcrossMonths).'</td>';
  print '<td align="right"><strong>'.sprintf('%.2f',$totalStipendPayment[0]).'</td>';
  print '<td align="right"><strong>'.sprintf('%.2f',$totalPaymentAcrossMonths).'</td>';
  print '</tr>';
  
  print '</table>';
}
?>