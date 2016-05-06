CREATE TABLE Host(
	ip VARCHAR(128) NOT NULL PRIMARY KEY,
	disk BIGINT NOT NULL,
	capable_disk BIGINT NOT NULL,
	cpu INT NOT NULL,
	capable_cpu INT NOT NULL,
	mem BIGINT NOT NULL,
	capable_mem BIGINT NOT NULL,
	gross_vm_cnt INT NOT NULL,
	live_vm_cnt INT NOT NULL
);

CREATE TABLE VM(
	name VARCHAR(64) NOT NULL PRIMARY KEY,
	ip VARCHAR(128),
	port INT,
	alloc_disk BIGINT NOT NULL,
	req_disk BIGINT NOT NULL,
	vec_disk INT NOT NULL,
	alloc_cpu INT NOT NULL,
	req_cpu INT NOT NULL,
	vec_cpu INT NOT NULL,
	alloc_mem BIGINT NOT NULL,
	req_mem BIGINT NOT NULL,
	vec_mem INT NOT NULL,
	is_run BOOLEAN NOT NULL
);

CREATE TABLE LOG(
	log_num INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
	name VARCHAR(64) NOT NULL,
	sequence INT NOT NULL,
	date_time DATETIME NOT NULL,
	disk_usage float(5, 2) NOT NULL,
	cpu_usage float(5, 2) NOT NULL,
	mem_usage float(5, 2) NOT NULL,
	CONSTRAINT log_fk
	FOREIGN KEY (name)
	REFERENCES VM (name) 
	ON DELETE CASCADE
);




