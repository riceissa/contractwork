create table stipends(
  stipend_id int(11) not null auto_increment primary key,
  payee varchar(40),
  payment float(7,2) not null,
  stipend_start_date date NOT NULL,
  stipend_end_date date NOT NULL,
  stipend_award_date date,
  notes varchar(2000)
) ENGINE=InnoDB AUTO_INCREMENT=15239276 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

insert into stipends(payee, payment, stipend_start_date, stipend_end_date, stipend_award_date, notes) values
  ('Issa Rice',700,'2016-02-01','2016-02-28',NULL,'Deferred to LiftIgniter milestone'),
  ('Issa Rice',900,'2016-03-01','2016-03-31',NULL,'Deferred to LiftIgniter milestone'),
  ('Issa Rice',900,'2016-04-01','2016-04-30',NULL,'Deferred to LiftIgniter milestone'),
  ('Issa Rice',1000,'2016-05-01','2016-05-31',NULL,'Deferred to LiftIgniter milestone'),
  ('Issa Rice',1250,'2016-06-01','2016-06-30',NULL,'Deferred to LiftIgniter milestone'),
  ('Issa Rice',1250,'2016-07-01','2016-07-31',NULL,'Deferred to LiftIgniter milestone'),
  ('Issa Rice',960,'2016-08-01','2016-08-31',NULL,'Deferred to LiftIgniter milestone; pay less than usual 1250 because of reduced work while in Japan'),
  ('Issa Rice',1250,'2016-09-01','2016-09-30',NULL,'Deferred to LiftIgniter milestone'),
  ('Issa Rice',1250,'2016-10-01','2016-10-31',NULL,'Deferred to LiftIgniter milestone'),
  ('Issa Rice',750,'2016-11-01','2016-11-30',NULL,'Deferred to LiftIgniter milestone');

insert into stipends(payee, payment, stipend_start_date, stipend_end_date, stipend_award_date, notes) values
  ('Issa Rice',300,'2016-05-01','2016-05-31','2016-05-31',NULL),
  ('Issa Rice',250,'2016-06-01','2016-06-30','2016-06-30','Distribution changed from 300 + 1000 to 250 + 1250 to conserve cash'),
  ('Issa Rice',250,'2016-07-01','2016-07-31','2016-07-31',NULL),
  ('Issa Rice',190,'2016-08-01','2016-08-31','2016-08-31','Pay less than usual 250 because of reduced work while in Japan'),
  ('Issa Rice',250,'2016-09-01','2016-09-30','2016-09-30',NULL),
  ('Issa Rice',250,'2016-10-01','2016-10-31','2016-10-31',NULL),
  ('Issa Rice',250,'2016-11-01','2016-11-30','2016-11-30',NULL);

insert into stipends(payee, payment, stipend_start_date, stipend_end_date, stipend_award_date, notes) values
  ('Alex K. Chen',0.02,'2015-04-01','2015-12-31','2015-12-31','Fudge factor correction');