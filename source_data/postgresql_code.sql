create table if not exists course
(
    Id                int primary key,
    courseId          varchar(10) not null unique,
    courseCredit      int         not null,
    totalCapacity     int         not null,
    courseHour        int         not null,
    courseName        varchar     not null,
    courseDept        varchar     not null,
    requisiteCourseId varchar
);

create table if not exists class
(
    Id        int primary key,
    courseId  int,
    className varchar not null,
    constraint fk_courseId foreign key (courseId) references course (Id)
);

create table if not exists classList
(
    Id        serial primary key,
    classId   int,
    weekList  varchar not null,
    location  varchar,
    classTime varchar not null,
    weekDay   int     not null,
    constraint fk_courseId foreign key (classId) references class (Id)
);

create table if not exists teacher
(
    Id      serial primary key,
    classId int unique,
    name    varchar,
    constraint fk_courseId foreign key (classId) references class (Id)
);

create table if not exists department
(
    id           serial primary key,
    chinese_name varchar(20) unique not null,
    english_name varchar(40) unique
);

create table if not exists student
(
    id             serial primary key,
    name           varchar(10) not null,
    sex            char(1)     not null,
    department_id  int,
    student_number int         not null unique,
    constraint dep_for_key foreign key (department_id) references department (id)
);

create table if not exists course_selection
(
    id         serial primary key,
    student_id int not null,
    course_id  int not null,
    constraint stu_for_key foreign key (student_id) references student (id),
    constraint cor_for_key foreign key (course_id) references course (id)
);