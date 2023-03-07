/*
//javascript.js
//set map options
var myLatLng = { lat: -23.5489, lng: -46.6388 };
var mapOptions = {
    center: myLatLng,
    zoom: 7,
    mapTypeId: google.maps.MapTypeId.ROADMAP
};

//create map
var map = new google.maps.Map(document.getElementById('googleMap'), mapOptions);

//create a DirectionsService object to use the route method and get a result for our request
var directionsService = new google.maps.DirectionsService();

//create a DirectionsRenderer object which we will use to display the route
var directionsDisplay = new google.maps.DirectionsRenderer();

//bind the DirectionsRenderer to the map
directionsDisplay.setMap(map);

//define calcRoute function
function calcRoute() {
    //create request
    var request = {
        origin: document.getElementById("from").value,
        destination: document.getElementById("to").value,
        travelMode: google.maps.TravelMode.DRIVING, //WALKING, BYCYCLING, TRANSIT
        unitSystem: google.maps.UnitSystem.IMPERIAL
    }

    //pass the request to the route method
    directionsService.route(request, function (result, status) {
        if (status == google.maps.DirectionsStatus.OK) {

            //Get distance and time
            const output = document.querySelector('#output');
            output.innerHTML = "<div class='alert-info'>From: " + document.getElementById("from").value + ".<br />To: " + document.getElementById("to").value + ".<br /> Driving distance <i class='fas fa-road'></i> : " + result.routes[0].legs[0].distance.text + ".<br />Duration <i class='fas fa-hourglass-start'></i> : " + result.routes[0].legs[0].duration.text + ".</div>";

            //display route
            directionsDisplay.setDirections(result);
        } else {
            //delete route from map
            directionsDisplay.setDirections({ routes: [] });
            //center map in London
            map.setCenter(myLatLng);

            //show error message
            output.innerHTML = "<div class='alert-danger'><i class='fas fa-exclamation-triangle'></i> Could not retrieve driving distance.</div>";
        }
    });

}



//create autocomplete objects for all inputs
var options = {
    types: ['(cities)']
}

var input1 = document.getElementById("from");
var autocomplete1 = new google.maps.places.Autocomplete(input1, options);

var input2 = document.getElementById("to");
var autocomplete2 = new google.maps.places.Autocomplete(input2, options);
*/

//var myLatLng = { lat: -23.5489, lng: -46.6388 };
var mapOptions = {
    //center: myLatLng,
    zoom: 7,
};

function calcRoute(){
    const directionsService = new google.maps.DirectionsService();
    const directionsRenderer = new google.maps.DirectionsRenderer({
        draggable: true
    });

    const map = new google.maps.Map(document.getElementById('googleMap'), mapOptions);

    let varTeste = fetch("dados.json").then((response) => {
        response.json().then((dados) => {
            meuArray = [];
            varTeste = dados.rotas;

            posInicial = varTeste[0];
            meuArray.push({location: varTeste[0], stopover: true});

            for(let i = 1; (i < varTeste.length) && (varTeste[i] != posInicial) && (i < 27); i++){
                meuArray.push({location: varTeste[i], stopover: true});
            }

            console.log(meuArray);
            
            // Nao coloco 0 como sendo a ultima posicao do meu array para facilitar a leitura do mapa
            let destino = meuArray[meuArray.length - 1].location;
            meuArray.pop();
        
            directionsRenderer.setMap(map);
        
            directionsService.route({
                origin: '41.39753660 2.12356330',
                destination: destino,
        
                // coloco todos os ponsto aqui1
                waypoints: meuArray,
                travelMode: google.maps.TravelMode.DRIVING
            }).then(response => {
                console.log({response});
                directionsRenderer.setDirections(response);
            }).catch(err => {
                console.log({err});
            });

        });
    }).catch((error) => {
        const output = document.querySelector('#output');
        output.innerHTML = "<div><div class='alert-primary' role='alert'>Não foi possível carregar o mapa!</div>Lembre-se, esta aplicação está usando arquivos JSON. Por esse motivo, é necessário iniciar um servidor para que não haja um bloqueio pela politica CORS.<br/>Você pode iniciar um servidor usando aplicações como o XAMP ou o próprio Visual Studio Code, com o Live Server.</div>";
    });
    
}