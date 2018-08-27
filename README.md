MammalCam
-----
### Motion Activated WiFi Camera Made with the Adafruit Feather M0

This is a firmware application written in C++ using <a href="https://github.com/platformio/platformio-pkg-framework-arduinosam"> Arduino</a>
and<a href="https://github.com/adafruit/"> Adafruit</a> libraries. It sleeps
in low power mode until, activated by the motion sensor, it takes a picture.
Images are posted to <a href="https://learn.adafruit.com/adafruit-io/overview#">
io.adafruit.com</a> using the <a href="https://io.adafruit.com/api/docs/">REST
API</a>.

![Motion Camera](/images/image22.jpg)

#### Motivation
I've long wanted to play with an automatic wildlife camera just for the
fun of it. We have lots of deer in our neighborhood and this camera is intended
to take pictures of them eating my wife's garden.

There's also the fact that this project uses a TTL serial camera that speaks the
<a href="https://cdn-shop.adafruit.com/datasheets/VC0706protocol.pdf">VC0706
protocol</a>. Programming the device driver gave me the opportunity to learn how
to command the camera to:
  * Reset,
  * Display the boot message and firmware version,
  * Change baud rates,
  * Change the JPEG compression,
  * Take a picture, and
  * Upload pictures.


#### Materials
Here's a list of all the parts that went into this project. (Some items were
absent the day I took the group photo.)

![Motion Camera Exploded View](/images/image01.jpg)
  * <a href="https://www.adafruit.com/product/613">Weatherproof TTL Serial JPEG
  Camera with NTSC Video and IR LEDs</a>
  * <a href="https://www.adafruit.com/product/2884">FeatherWing Proto</a>
  * <a href="https://www.adafruit.com/product/1903">PowerBoost 500 Basic</a>
  * <a href="https://www.adafruit.com/product/189">PIR (Motion) Sensor</a>
  * <a href="https://www.adafruit.com/product/353">Lithium Ion Battery Pack</a>
  * <a href="https://www.adafruit.com/product/761">Cable Gland PG-9</a>
  * <a href="https://www.adafruit.com/product/610">Panel Mount 2.1mm DC barrel jack</a>
  * <a href="https://www.adafruit.com/product/2697">USB to 2.1mm Male Barrel Jack Cable</a>
  * <a href="http://a.co/d/dMs9XEk">3.2" x 5.1" x 2.8" (80mmx130mmx70mm) ABS
  Universal Project Enclosure with Polycarbonate Transparent Cover</a>
  * <a href="https://www.mouser.com/ProductDetail/Central-Semiconductor/PN2907A?qs=sGAEpiMZZMshyDBzk1%2fWiw99kSkYzPxmkFvzASyUUU8%3d">PN2907A Bipolar Junction Transistors (BJT) -
  PNP General Purpose Switch</a>
  * <a href="https://www.adafruit.com/product/3010">Adafruit Feather M0 WiFi -
  ATSAMD21 + ATWINC1500</a>
  * <a href="https://www.adafruit.com/product/2940">Short Headers Kit for
  Feather - 12-pin + 16-pin Female Headers</a>
  * <a href="https://www.adafruit.com/product/3299">M2.5 Black Nylon Screw Set</a>
  * <a href="https://www.adafruit.com/product/261">JST PH (2mm) 2-Pin Cable - Female
  Connector 100mm</a>
  * 10K Ohm resistor 5%
  * <a href="http://a.co/d/dXckbb6">Cinch straps</a>
  * Foam scraps, cardboard, hot glue, epoxy, #10 wood screws,


#### Schematic
The electrical connections are shown below.
![Motion Camera Schematic](/images/motion_camera_schematic.svg)


#### How to Build the Motion Camera
<ol>
<li><p>I've found that the easiest method for mounting circuit boards inside
plastic enclosures is to use plastic stand-offs and epoxy. Use small screws on
the top and stand-offs below.</p>

![Feather M0, FeatherWing Proto, PowerBoost 500 Basic, PN2907A PNP Transistor](/images/image02.jpg)</li>


<li><p>Clean all the parts thoroughly with rubbing alcohol and a
clean cloth. To install, simply dab a little epoxy on the stand-offs and then set
the part into the enclosure. Be sure you know where you want everything to go
before you start gluing. Once you stick them in they're very hard to remove.</p>

![Circuit Board Mounting with M2.5 Hardware and Epoxy](/images/image12.jpg)
</li>


<li><p>Place the battery into the enclosure as shown. The cord and the Feather's
battery terminal must be on the same side or the cord won't reach. Cut a bit of
cardboard to just sit on top of the battery. Cut off a corner to allow the cord
to get by. Hot glue the cardboard to the sides of the enclosure.</p>

![Battery Pack Cardboard Retainer](/images/image03.jpg)</li>


<li><p>Cut some scrap foam to sit on top of the battery. It should compress
slightly when you attach the lid to the enclosure. The pressure should be just
enough to keep the battery still without pinching it. Be gentle with it.</p>

![Battery Pack Foam Retainer](/images/image04.jpg)</li>


<li><p>You will need to drill a hole in the enclosure for the DC jack. Ideally
the hole should be just big enough to allow the jack to pass through. Seal around
the jack with silicone sealant or bathtub caulk to prevent leaks.</p>

![Panel Mount 2.1mm DC barrel jack](/images/image05.jpg)</li>


<li><p>Here's a close-up showing a bit of caulk around the DC jack.</p>

![Mounting the DC Jack & Cable Gland](/images/image10.jpg)</li>


<li><p>Drill a hole in the enclosure for the cable gland. Ideally the hole should
be just big enough to allow the gland to pass through. When you insert the gland
into its hole it should sit on top of the o-ring provided. This provides a
water tight seal.</p>

![Cable Gland PG-9](/images/image06.jpg)</li>


<li><p>Drill a hole in the enclosure for the camera mounting screw. Ideally the
hole should be just big enough to allow the screw to pass through. The best fit
will require you to use a screwdriver to thread the screw into the new hole. Be
sure to place the hole far enough from the front that the camera can sit flat on
top of the enclosure and swivel smoothly from side to side.</p>

![#10-32 x 1" Camera Mounting Hardware](/images/image07.jpg)</li>


<li><p>For the larger holes a step-drill really helps. This is what tradespersons
use for this task. Unlike a twist drill a step-drill only removes a small amount of
material at a time. If you must use a twist drill, make a series of holes stepping
up through your drill library until you reach the final size. If you try to
remove too much material at once the bit will bite hard into the plastic, get
stuck, and jerk the drill out of your hand.</p>

![Step Drill](/images/image09.jpg)</li>


<li><p>Once the DC jack is mounted, wire it up to the FeatherWing. The positive
terminal of the DC jack is the one in the center. Connect this terminal to the USB
terminal on the FeatherWing. The DC jack will feed the Feather to charge the battery.
Connect one of the outer DC jack terminals to the ground bus on the FeatherWing.</p>

![Connecting the FeatherWing to the DC Jack](/images/image08.jpg)</li>


<li><p>Connect the FeatherWing's BAT and GND terminals to the JST PH 2-Pin Cable
and plug that into the PowerBoost 500 Basic. If you order this part from Adafruit
the pins will be in the correct polarity. If you got it elsewhere, be sure to check the
polarity and correct it before use.</p>

![Connecting the FeatherWing to the PowerBoost 500 Basic](/images/image11.jpg)</li>


<li><p>Cut off the excess cable on the camera. Leave enough extra on the inside
of the enclosure that that you can remove the circuit boards with wires attached
and not have to adjust the gland.  Connect the green and white wires to TX and RX
as shown.</p>

![Connecting the Weatherproof TTL Serial JPEG Camera to Power and Data](/images/image13.jpg)</li>


<li><p>Stow the brown and yellow wires (NTSC) away in the bottom of the enclosure.
You might someday be able to stream this signal over WiFi for a LiveCam project so
preserve them for the future.</p>

![Store the Camera NTSC Wires Out of the Way](/images/image16.jpg)</li>


<li><p>Connect the red wire to 5V on the PowerBoost.</p>

![Shunt Connecting EN and LBO](/images/image14.jpg)</li>

<li><p>Connect the black wire to pin 1 (emitter) of the PN2907A. Connect it's pin
2 (base) to PIN_A0. Connect it's pin 3 (collector) to the GND bus. <a href="https://www.mouser.com/datasheet/2/68/pn2906-pm2907a-27764.pdf">According
to the datasheet</a>, this transistor can sink up to 600 mA. That's more than
enough to power the camera even with it's IR lights on. (Sink is another word for
the place where current exits a system.)</p>

![Sinking the Camera Through the PN2907A PNP Transistor](/images/image15.jpg)</li>

<li><p>Connect the PIR to 5V, and GND. Install a wire, or a pair of male headers
with a shunt, on the EN and LBO pins. This will shutdown the PowerBoost when the
battery goes low.</p>

![Connecting the Camera to 5V, and GND](/images/image17.jpg)</li>


<li><p>Connecting the PIR output to PIN_A1. Also connect a 10K Ohm resistor (not
shown) from this pin to the second ground pin on the DC jack. Set the <a href="https://learn.adafruit.com/pir-passive-infrared-proximity-motion-sensor/testing-a-pir">retriggering</a>
jumper on the PIR in the L position to prevent retriggering.
Adjust the sensitivity to the low side of the potentiometer. If you're wondering
why, I have two reasons. One, the camera only has a ten foot range in night mode.
And two, when using the unit outdoors there's more variability in the background
temperature and this allows for more (false positive) triggering. To learn more
checkout <a href="https://learn.adafruit.com/pir-passive-infrared-proximity-motion-sensor/how-pirs-work">
this article</a>.</p>

![Connecting the PIR Output to PIN_A1](/images/image18.jpg)</li>


<li><p>The first time around I had the PIR motion sensor inside the enclosure. It
was less work to put it there and being inside meant it would stay dry.</p>

![Assembled Iteration Zero](/images/image19.jpg)</li>


<li><p>I soon discovered that the polycarbonate cover either absorbs or reflects
IR light. (The sensor was blind.) So I drilled a hole in the cover and epoxied
the IR dome into it.</p>

![Assembled Iteration One](/images/image20.jpg)</li>


<li><p>Screw the straps to the back of the enclosure using the holes provided.</p>

![Attach Mounting Straps with #10 x0.5" Wood Screws](/images/image21.jpg)</li>


<li><p>Strap the camera to a convenient tree and enjoy the pictures.</p>

![Motion Camera Mounted on a Cedar](/images/image22.jpg)</li>
</ol>

#### Building the Code with the Arduino IDE
1. Checkout the arduino branch of the project.
```bash
$ git checkout arduino
```
2. Edit `src/motion_camera_secrets.hpp` and replace the placeholder text with your
own secrets.
3. Try to build the project. If you get compiler errors like this:
```
    undefined reference to 'std::ios_base::Init::~Init()'
```
and, a lot more saying undefined reference to something in the namespace `std::`,
it means that the linking step couldn't find the C++ Standard Library. To solve
this, edit
```bash
${INSTALL_FOLDER}/Arduino15/packages/adafruit/hardware/samd/1.2.1/platform.txt
```
4. Locate this line:
```ini    
compiler.c.elf.cmd=arm-none-eabi-gcc
```
5. Change the line to:
```ini
compiler.c.elf.cmd=arm-none-eabi-g++
```
You should now be able to compile and link without further problems.


#### Building the Code with Platform IO

1. Checkout the __platformio__ branch of the project.
  ```bash
  $ git checkout platformio
  ```

1. Create or modify your __platformio.ini__ to include
  ```ini
  [platformio]
  ; MacOS X / Linux
  lib_extra_dirs =
      ${env.HOME}/path/to/your/Arduino/libraries

  [common]
  lib_deps_external =

  [env:adafruit_feather_m0]
  platform = atmelsam
  board = adafruit_feather_m0
  framework = arduino
  upload_port = /dev/cu.your_modem_device
  upload_speed = 115200
  lib_deps =
      ${common.lib_deps_external}
  build_flags =
      '-DWIFI_SSID="Your WiFi SSID"'
      '-DWIFI_PASS="Your WiFi password'
      '-DAIO_USERNAME="Your Adafruit user name"'
      '-DAIO_KEY="our secret AIO key"'
  ```


#### Installing SSL Certificates
If you're like me and you'd like to keep your AIO secret key a secret then you'll
want to connect to the AIO REST API using SSL. (All the code in this project
  assumes that you do.) To do that simply load the io.adafruit.com SSL certificate
  into the ATWINC1500. See <a href="https://learn.adafruit.com/adafruit-atwinc1500-wifi-module-breakout?view=all#updating-ssl-certificates">these directions</a> to learn how.

#### Image Colors
You may be wondering why the leaves and grass look gray. It's because the  camera
is particularly sensitive to infrared light. It has to be to work in night mode.

![Sample Image](/images/sample_image.png)


#### Use Wisely
It's not polite to spy on people without notice. Be sure to let people know you're
operating the camera and sending the pictures to the Internet so that they can use
common sense when entering the area where the camera is active.

![Warning; Security Cameras in Use!](/images/security-cameras-in-use.jpg)
