<?php
$curl = curl_init();
curl_setopt_array($curl, array(
    // return as string
    CURLOPT_RETURNTRANSFER => 1,
    CURLOPT_CONNECTTIMEOUT => 5,
    CURLOPT_URL => 'http://blynk-cloud.com:8080/d86b402e16154023abb70d6e6ef03575/get/V20'
));

$response = curl_exec($curl);
$intReturnCode = curl_getinfo($curl, CURLINFO_HTTP_CODE);
curl_close($curl);

if ($intReturnCode == 200) {
    $replace = array('[', ']', '"');
    $clearString = str_replace($replace, '', $response);
    $values = explode(",", $clearString);

    $temperature = round(floatval($values[0]), 1);
    $humidity = intval($values[1]);

    echo "<div>Teplota: <b>" . $temperature . "°C</b>" . "<br />Vlhkost: <b>". $humidity . "%</b></div>";
}

?> 