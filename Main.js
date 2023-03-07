function calcRoute(){
    const directionsService = new google.maps.DirectionsService();
    const directionsRenderer = new google.maps.DirectionsRenderer({
        draggable: true
    });

    const map = new google.maps.Map(document.getElementById('googleMap'), { zoom: 7, });

    let pontos = fetch("dados.json").then((response) => {
        response.json().then((dados) => {
            let meuArray = [];
            let pontos = dados.rotas;

            // Marco a primeira posicao como sendo o inicio de pontos
            // Logo em seguida, removo essa primeira posicao
            let posInicial = pontos[0];
            pontos.shift();

            /*
             * Nota:
             * Por questao de melhor visualizacao, foram colocador limites na quantidade de pontos colocados no mapa
             * (i < 27) impede que sejam colocados mais de 26 pontos (numero de letras do alfabeto)
             * (pontos[i] != posInicial) impede que sejam tracadas mais de uma rota simultaneamente
             * Devido a essas restricoes, o mapa nao mostra o retorno do ultimo vertice visitado para o ponto de demanda 0
             */
            for(let i = 0; (i < pontos.length) && (pontos[i] != posInicial) && (i < 27); i++){
                meuArray.push({location: pontos[i], stopover: true});
            }
            
            // Marco o ultimo ponto como o destino do meu caminho
            // Todos os outros pontos (exceto o primeiro) sao paradas marcadas como paradas intermediarias (waitpoints)
            let destino = meuArray[meuArray.length - 1].location;
            meuArray.pop();

            directionsRenderer.setMap(map);
        
            directionsService.route({
                origin: posInicial,
                destination: destino,
        
                // Coloco todas as paradas aqui
                waypoints: meuArray,
                travelMode: google.maps.TravelMode.DRIVING
            }).then(response => {
                console.log({response});
                directionsRenderer.setDirections(response);
            }).catch(err => {
                console.log({err});
            });

            const output = document.querySelector('#output');
            output.innerHTML = "<div>Mapa carregado com sucesso!</div>";

        });
    }).catch((error) => {
        const output = document.querySelector('#output');
        output.innerHTML = "<div><div class='alert-primary' role='alert'>Não foi possível carregar o mapa!</div>Lembre-se, esta aplicação está usando arquivos JSON. Por esse motivo, é necessário iniciar um servidor para que não haja um bloqueio pela politica CORS.<br/>Você pode iniciar um servidor usando aplicações como o XAMP ou o próprio Visual Studio Code, com a extensao Live Server.</div>";
    });
    
}