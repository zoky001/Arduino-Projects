$(document).ready(function () {
    var ime = "";
var razmak = "_";

    $("#loginButton").click(function () {
        ime = $("#email").val();
//myFunction(ime);
fancy(ime);

    });

   // setInterval(function(){ alert("Hello"); }, 3000);

//analog/0
  //  setInterval(function(){ alert("Hello"); }, 3000);

    function myFunction(text) {
        var oReq = new XMLHttpRequest();
        oReq.open("GET","http://10.10.9.45/cmd/T0"+text);
        console.log(ime);
        oReq.send();
       // setInterval(function(){ alert("Hello"); }, 3000);
    }

    function myFunctionRAzmak(text) {
        console.log("salje");
        razmak = razmak+"_";
        var oReq = new XMLHttpRequest();
        oReq.open("GET","http://10.10.9.45/cmd/T0"+razmak + text);
        console.log(ime);
        oReq.send();
        // setInterval(function(){ alert("Hello"); }, 3000);
    }

    function fancy(text) {
        setInterval(myFunctionRAzmak(text), 10);
    }

   // salje text
    setInterval(function(){
       // myFunctionRAzmak("bok");

        //alert("Hello");



    }, 500);


function ispisTipke(text) {
    var i = parseInt(text);
    if (i < 470 && i > 370){

        console.log("Pritisnuta je tipka LEFT!!");
        document.getElementById("demo").innerHTML =
            "Pritisnuta je tipka LEFT!!";
        placeDiv(2);

    }
    else if(i > 600 && i < 690){
        console.log("Pritisnuta je tipka LEFT!!");
        document.getElementById("demo").innerHTML =
            "Pritisnuta je tipka SELECT!!";

    }
    else if(i > -1 && i < 50){
        console.log("Pritisnuta je tipka LEFT!!");
        document.getElementById("demo").innerHTML =
            "Pritisnuta je tipka RIGHT!!";
        placeDiv(0);

    }
    else if(i > 210 && i < 310){
        console.log("Pritisnuta je tipka LEFT!!");
        document.getElementById("demo").innerHTML =
            "Pritisnuta je tipka DOWN!!";
        placeDiv(1);

    }
    else if(i > 80 && i < 130){
        console.log("Pritisnuta je tipka LEFT!!");
        document.getElementById("demo").innerHTML =
            "Pritisnuta je tipka UP!!";
        placeDiv(3);

    }
    else {
        console.log("Pritisnuta je tipka!!");
        document.getElementById("demo").innerHTML =
            "NIŠTA NIJE PRITISNUTO";
    }
}

    // provjerava tipku
  /*  setInterval(function(){
        console.log("Dohvacam podatek...");

        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
            if (this.readyState == 4 && this.status == 200) {

                console.log("REzultat: " +  this.responseText);



              var i = parseInt(this.responseText);
                if (i < 470 && i > 370){

                  console.log("Pritisnuta je tipka LEFT!!");
                    document.getElementById("demo").innerHTML =
                        "Pritisnuta je tipka LEFT!!";

              }
              else if(i > 600 && i < 690){
                    console.log("Pritisnuta je tipka LEFT!!");
                    document.getElementById("demo").innerHTML =
                        "Pritisnuta je tipka SELECT!!";

                }
                else if(i > -1 && i < 50){
                    console.log("Pritisnuta je tipka LEFT!!");
                    document.getElementById("demo").innerHTML =
                        "Pritisnuta je tipka RIGHT!!";

                }
                else if(i > 210 && i < 310){
                    console.log("Pritisnuta je tipka LEFT!!");
                    document.getElementById("demo").innerHTML =
                        "Pritisnuta je tipka DOWN!!";

                }
                else if(i > 80 && i < 130){
                    console.log("Pritisnuta je tipka LEFT!!");
                    document.getElementById("demo").innerHTML =
                        "Pritisnuta je tipka UP!!";

                }
              else {
                    console.log("Pritisnuta je tipka!!");
                    document.getElementById("demo").innerHTML =
                        "NIŠTA NIJE PRITISNUTO";
                }

            }
        };
        xhttp.open("GET", "http://10.10.9.45/analog/0", true);
        xhttp.send();



        //alert("Hello");



    }, 500);
    */

    //web socket
    var sockeet = new WebSocket("ws://10.10.9.45");
    sockeet.onmessage = function (event) {

        try {
            console.log(JSON.parse(event.data).value);
            ispisTipke(JSON.parse(event.data).value);

        }
        catch (e){

        }
    }

    sockeet.onopen = function (event) {
        sockeet.send("SA00");
    }

    var hrana = document.getElementById('hrana');
    hrana.style.position = "absolute";
    hrana.style.left = 200+'px';
    hrana.style.top = 200+'px';



var top = 0;
    var left = 0;
    //zmija
    function placeDiv(poz) {
        var d = document.getElementById('demo');

        if(poz == 0){

            top++;


        }
        else if(poz == 1){
            left++;
        }
        else if(poz == 2){
            top--;

        }
        else if(poz == 3){
            left--;

        }
        console.log("Pozicija: " + d.style.left);
        d.style.position = "absolute";
      d.style.left = top+'px';
      d.style.top = left+'px';

      if (top > 180 && top < 220 && left > 180 && left < 220){

          document.getElementById("pojeo").innerHTML =
              "POJEO SI!!";

      }

    }





});