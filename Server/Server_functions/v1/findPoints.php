<?php 
 
	/*
	* Created by Belal Khan
	* website: www.simplifiedcoding.net 
	* Retrieve Data From MySQL Database in Android
	*/
	
	//database constants
	define('DB_HOST', 'localhost');
	define('DB_USER', 'root');
	define('DB_PASS', '');
	define('DB_NAME', 'odj2');
	
	//connecting to database and getting the connection object
    $conn = new mysqli(DB_HOST, DB_USER, DB_PASS, DB_NAME);
    
    if($_SERVER['REQUEST_METHOD']=='POST'){
        if(   isset($_POST['Meetings_id']))
            {
            //operate the data further 
            //creating a query
            $stmt = $conn->prepare("SELECT id, tim, sub, About FROM points WHERE Meetings_id=? ;");
            $stmt->bind_param("s", $_POST['Meetings_id']);
            $stmt->execute(); 
            $stmt->bind_result($id, $time, $sub, $About);


            $entreprises = array(); 
	
	//traversing through all the result 
	    while($stmt->fetch()){
		    $temp = array();
		    $temp['id'] = $id; 
            $temp['Time'] = $time; 
            $temp['Subject'] = $sub; 
		    $temp['About'] = $About;
		    array_push($entreprises, $temp);
	    }
    
        }else{
            $response['error'] = true; 
            $response['message'] = "Required fields are missing";
        }
    }else{
        $response['error'] = true; 
        $response['message'] = "Invalid Request";
    }
    
    echo json_encode($entreprises);
	
	