<?php

	/**
	 * 
	 */
	class DbOperations
	{
		private $con;

		function __construct()
		{
			require_once dirname(__FILE__).'/DbConnect.php';

			$db = new DbConnect();

			$this->con = $db->connect();
		}
		
		public function createUser($username, $pass, $email,$token){
			if($this->isUserExist($username,$email)){
				return -1; 
			}else{
				$password = md5($pass);
				$stmt = $this->con->prepare("INSERT INTO `employees` (`id`, `username`, `password`, `email`,`token`,`Entreprises_id`) VALUES (NULL, ?, ?, ?, ?, 1);");
				$stmt->bind_param("ssss",$username,$password,$email,$token);

				if($stmt->execute()){
					return $stmt->insert_id;
				}else{
					return -2; 
				}
			}
		}
		public function userLogin($username, $pass){
			$password = md5($pass);
			$stmt = $this->con->prepare("SELECT id FROM employees WHERE username = ? AND password = ?");
			$stmt->bind_param("ss",$username,$password);
			$stmt->execute();
			$stmt->store_result(); 
			return $stmt->num_rows > 0; 
		}

		public function getUserByUsername($username){
			$stmt = $this->con->prepare("SELECT * FROM employees WHERE username = ?");
			$stmt->bind_param("s",$username);
			$stmt->execute();
			return $stmt->get_result()->fetch_assoc();
		}
		public function getUserById($id){
			$stmt = $this->con->prepare("SELECT * FROM employees WHERE id = ?");
			$stmt->bind_param("s",$id);
			$stmt->execute();
			return $stmt->get_result()->fetch_assoc();
		}
		private function isUserExist($username, $email){
			$stmt = $this->con->prepare("SELECT id FROM employees WHERE username = ? OR email = ?");
			$stmt->bind_param("ss", $username, $email);
			$stmt->execute(); 
			$stmt->store_result(); 
			return $stmt->num_rows > 0; 
		}

		public function createMeeting($name, $date, $desc,$Entreprises_id){

				$stmt = $this->con->prepare("INSERT INTO `meetings` (`id`, `Nam`, `Dat`, `Descr`,`Entreprises_id`) VALUES (NULL, ?, ?, ?, ?);");
				$stmt->bind_param("ssss",$name,$date,$desc,$Entreprises_id);

				if($stmt->execute()){
					/*$stmt = $this->con->prepare("SELECT LAST_INSERT_ID()");
					$stmt->execute();
					return $stmt->get_result()->fetch_assoc(); */
					return $stmt->insert_id;
				}else{
					return 2; 
				}
		}

		public function createPoint($time, $subject, $about, $point_id){

			$stmt = $this->con->prepare("INSERT INTO `points` (`id`, `tim`, `sub`, `About`,`Meetings_id`) VALUES (NULL, ?, ?, ?, ?);");
			$stmt->bind_param("ssss",$time,$subject,$about,$point_id);

			if($stmt->execute()){
				return 1; 
			}else{
				return 2; 
			}
	}
	public function createEntreprise($name){

		$stmt = $this->con->prepare("INSERT INTO `entreprises` (`id`, `Names`) VALUES (NULL, ?);");
		$stmt->bind_param("s",$name);

		if($stmt->execute()){
			return 1; 
		}else{
			return 2; 
		}
}
//getting all tokens to send push to all devices
public function getAllTokens(){
	$stmt = $this->con->prepare("SELECT token FROM employees");
	$stmt->execute(); 
	$result = $stmt->get_result();
	$tokens = array(); 
	while($token = $result->fetch_assoc()){
		array_push($tokens, $token['token']);
	}
	return $tokens; 
}

//getting a specified token to send push to selected device
public function getTokenByUsername($username){
	$stmt = $this->con->prepare("SELECT token FROM employees WHERE username = ?");
	$stmt->bind_param("s",$username);
	$stmt->execute(); 
	$result = $stmt->get_result()->fetch_assoc();
	return array($result['token']);        
}

public function getTokenByEntreprise($id){
	$stmt = $this->con->prepare("SELECT token FROM employees WHERE Entreprises_id = ?");
	$stmt->bind_param("s",$id);
	$stmt->execute(); 
	$result = $stmt->get_result()->fetch_assoc();
	return array($result['token']);        
}

//getting all the registered devices from database 
public function getAllDevices(){
	$stmt = $this->con->prepare("SELECT * FROM employees");
	$stmt->execute();
	$result = $stmt->get_result();
	return $result; 
}

	}