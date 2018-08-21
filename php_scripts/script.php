<?php
header('Content-Type: image/jpeg');

$username = 'admin';
$password = '123456';
$size = $_GET['size'];
$URL = 'http://91.187.58.40:999/mjpeg/snap.cgi?chn=' . $size;
$BLYNK_DATA_URL = 'http://blynk-cloud.com:8080/d86b402e16154023abb70d6e6ef03575/get/V20';
// vzaoral.ddns.net/mjpeg/snap.cgi?chn=

// get webcam picture
$curl = curl_init();
curl_setopt($curl, CURLOPT_URL, $URL);
curl_setopt($curl, CURLOPT_TIMEOUT, 30); //timeout after 30 seconds
curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);
curl_setopt($curl, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
curl_setopt($curl, CURLOPT_USERPWD, "$username:$password");
$status_code = curl_getinfo($curl, CURLINFO_HTTP_CODE);   //get status code
$response_image = curl_exec($curl);
curl_close($curl);

if ($size > 1) {
    echo $response_image;
} else {
    // get meteo data
    $curl = curl_init();
    curl_setopt_array($curl, array(
        // return as string
        CURLOPT_RETURNTRANSFER => 1,
        CURLOPT_CONNECTTIMEOUT => 5,
        CURLOPT_URL => $BLYNK_DATA_URL
    ));

    $response_meteo_data = curl_exec($curl);
    $intReturnCode = curl_getinfo($curl, CURLINFO_HTTP_CODE);
    curl_close($curl);

    if ($intReturnCode == 200) {
        $replace = array('[', ']', '"');
        $clearString = str_replace($replace, '', $response_meteo_data);
        $values = explode(",", $clearString);

        $temperature = round(floatval($values[0]), 1);
        $humidity = intval($values[1]);

   
        $jpg_image = imagecreatefromstring($response_image);
        $text = ' Teplota: ' . $temperature . '°C  Vlhkost: ' . $humidity . '%';
        $font =  dirname(__FILE__) . '/arial.ttf';
        $font_color = imagecolorallocate($jpg_image, 0, 0, 0);
        $bg_color = imagecolorallocatealpha($jpg_image, 220, 220, 220, 100);

        $font_size = $size == 1 ? 9 : 25;
        $offset_x = $size == 1 ? 470 : 1456;
        $offset_y = $size == 1 ? 35 : 85;
        $text_width = $size == 1 ? 200 : 455;
        $text_height = $size == 1 ? 20 : 40;
        $text_rectangle_offset = $size == 1 ? -2 : 2;

        imagefilledrectangle($jpg_image, $offset_x, $offset_y + $text_rectangle_offset, $offset_x + $text_width, $offset_y - $text_height, $bg_color);
        imagettftext($jpg_image, $font_size, 0, $offset_x, $offset_y - 7, $font_color, $font, $text);
        imagejpeg($jpg_image);
        imagedestroy($jpg_image);
    } else {
        echo $response_image;
    }
}
?> 