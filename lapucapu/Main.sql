create table if not exists GodFather
(
	user_id bigint not null auto_increment,
	tbl_name varchar(25) not null,
	haslo varchar(41) not null,
	primary key (user_id)
);

create table if not exists user_1
(
	nadawca bigint not null,
	czas timestamp default current_timestamp,
	przeczytana char(1) default 'n',
	wiadomosc text not null
);

create table if not exists user_2
(
	nadawca bigint not null,
	czas timestamp default current_timestamp,
	przeczytana char(1) default 'n',
	wiadomosc text not null
);

create table if not exists user_3
(
	nadawca bigint not null,
	czas timestamp default current_timestamp,
	przeczytana char(1) default 'n',
	wiadomosc text not null
);
