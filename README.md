3d yazıcı ile baskı alıp kendi evinizde geliştirebileceğiniz oyunkolu. 
Tasarımımda kendim arduino nano esp 32 kullandım siz kendiniz tercihinize göre sadece esp32 yada farklı mikroişlemciler kullanabilirsiniz. Lakin benim kodum gereği bluetooth entegresi olan bir kart seçilmelidir. 
Bu arduino kodu BleGamepad kütüphanesi ile çalışmaktadır. bu kütüphaneyi arduino IDE programına kurmanız gerekmektedir. Bu kod ile bluetooth bağlanltısı sağlanacak ve oyunkolu olarak tanımlanacaktır.
Kendi tasarımımda 2 adet joystick ve 4 adet buton kullandım daha fazla kullanmak sizin tercihiniz ben elimde olan malzemeler ile bir ürün çıkarmak istedim. 

bleGamepadConfig.setButtonCount(16);

bleGamepadConfig.setHatSwitchCount(0);

bleGamepadConfig.setWhichAxes(true, true, false, true, true, false, false, false);  // X, Y, Z, rZ aktif


bu kodlardan joystick iniz istediğiniz eksenleri hareket ettirmesini seçiyorsunuz. Buradaki true yada false değiştirerek sonuçları gözlemleyebilir buna görede hareket edebilirsiniz.

if (joy1Btn == LOW && joy1BtnPrev == HIGH) {

      Serial.println(F("Left Joystick Button"));
      
      bleGamepad.press(BUTTON_10);
      
      delay(debounceDelay);
      
    } else if (joy1Btn == HIGH && joy1BtnPrev == LOW) {
    
      bleGamepad.release(BUTTON_10);
      
    }

bu kodlardan ise butonların varsayılan gamepad butonlarından hangilerinin çalışmasını istediğinizi seçiyorsunuz. Burada bulunan BUTTON_10 yazısı BUTTON_1 ile BUTTON_17 arasında değiştirilebilmektedir. Aynı şekilde değiştirerek istediğiniz butonları aktif edebilirsiniz.

Joystick ve buton bağlantıları kodun başında arduino nano esp32 ye göre tanımlanmıştır. isterseniz oradakileri kullanabilir isterseniz değiştirebilirsiniz. 

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

A joystick you can print with a 3D printer and develop at home.
I used an Arduino Nano ESP 32 in my design. You can use only the ESP32 or other microprocessors of your choice. However, for my code, a board with Bluetooth integration must be selected.
This Arduino code works with the BleGamepad library. You need to install this library in the Arduino IDE. This code will establish the Bluetooth connection and define it as a joystick.
In my design, I used two joysticks and four buttons. Using more is your choice. I wanted to create a product with the materials I had on hand.

bleGamepadConfig.setButtonCount(16);
bleGamepadConfig.setHatSwitchCount(0);
bleGamepadConfig.setWhichAxes(true, true, false, true, true, false, false, false); // X, Y, Z, rZ active

With these codes, you select the desired axes for your joystick to move. You can observe the results by changing this to true or false and act accordingly.

if (joy1Btn == LOW && joy1BtnPrev == HIGH) {
Serial.println(F("Left Joystick Button"));
bleGamepad.press(BUTTON_10);
delay(debounceDelay);
} else if (joy1Btn == HIGH && joy1BtnPrev == LOW) {
bleGamepad.release(BUTTON_10);
}

With these codes, you select which of the default gamepad buttons you want to use. The text BUTTON_10 here can be changed between BUTTON_1 and BUTTON_17. You can activate the desired buttons by changing it in the same way.

The joystick and button connections are defined at the beginning of the code for the Arduino Nano ESP32. You can use them or change them if you wish.
