<?php
$url = file_get_contents("config/aws-backend-url");
if ( $url == false ) {
    echo json_encode( array('stat'=>false, 'msg'=>'Error reading url config file!') );
	return;
}

$jsonResponse = file_get_contents($url);
if ( $jsonResponse == false ) {
    echo json_encode( array('stat'=>false, 'msg'=>'Error requesting service!') );
    return;
}

$jsonObject = json_decode($jsonResponse);
if ( $jsonObject == false ) {
	$responseStat = false;
	$responseMsg = 'Error decoding json response!';
} else {
	//Possibilities:
	//  json = { "status":"error", "message":"details about error", "data":null }
	//  json = { "status":"success", "message":null, "data":"requested info" }
	if ( $jsonObject->{'status'} == 'success' ) {
		$responseStat = true;
		$responseMsg = $jsonObject->{'data'};
	} else {
		$responseStat = false;
		$responseMsg = $jsonObject->{'message'};
	}
}

echo json_encode( !isset($responseStat) ? array('stat'=>false, 'msg'=>'Error not handled!') : array('stat'=>$responseStat, 'msg'=>$responseMsg) );
 // //nonexistent node won't set $responseStat, for example
?>