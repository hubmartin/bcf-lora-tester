<a href="https://www.bigclown.com/"><img src="https://bigclown.sirv.com/logo.png" width="200" height="59" alt="BigClown Logo" align="right"></a>

# LoRaWAN tester build on open-source BigCLown kit

- Core Module
- LoRa Module
- LCD Module
- Battery Module

Tester can work without LCD Module. To get and set different DEVADDR, APPEUI and APPKEY, please change the code.

After start the JOIN request is sent. If the LED is not blinking after few seconds, then the request was sucessfull. By pressing the button the test packet is sent.

![My image](https://github.com/hubmartin/bcf-lora-tester/blob/master/lcd.jpg)



If you want to get more information about Core Module, firmware and how to work with it, please follow this link:

**https://www.bigclown.com/doc/firmware/basic-overview/**

User's application code (business logic) goes into `app/application.c`.
The default content works as a *Hello World* example.
When flashed into Core Module, it toggles LED state with each button press.

## License

This project is licensed under the [MIT License](https://opensource.org/licenses/MIT/) - see the [LICENSE](LICENSE) file for details.

---

Made with &#x2764;&nbsp; by [**HARDWARIO s.r.o.**](https://www.hardwario.com/) in the heart of Europe.
