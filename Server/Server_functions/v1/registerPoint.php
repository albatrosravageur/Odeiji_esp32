<?php 

require_once '../includes/DbOperations.php';

$response = array(); 

if($_SERVER['REQUEST_METHOD']=='POST'){
	if(
		isset($_POST['time']) and 
			isset($_POST['subject']) and 
                isset($_POST['about']) and
                    isset($_POST['meeting_id']))
		{
		//operate the data further 

		$db = new DbOperations(); 

		$result = $db->createPoint( 	$_POST['time'],
									$_POST['subject'],
                                    $_POST['about'],
                                    $_POST['meeting_id']
								);
		if($result == 1){
			$response['error'] = false; 
			$response['message'] = "Point registered successfully";
		}elseif($result == 2){
			$response['error'] = true; 
			$response['message'] = "Some error occurred please try again";			
		}

	}else{
		$response['error'] = true; 
		$response['message'] = "Required fields are missing";
	}
}else{
	$response['error'] = true; 
	$response['message'] = "Invalid Request";
}

echo json_encode($response);