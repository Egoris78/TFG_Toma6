using System.Collections;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;

public class Game : MonoBehaviour
{
    const int NUM_OF_PLAYERS = 4;
    // Start is called before the first frame update
    void Start()
    {
        game_deck = new Deck();
        game_players= new List<Player>();
        for (int i =0; i < NUM_OF_PLAYERS; i++)
        {
            game_players.Add(new Player());
        }
        Card cartaEj = new Card(1, 5);
       
        game_players[0].mano_player.addCard(cartaEj);
    }

    // Update is called once per frame
    void Update()
    {
        
    }
    Deck game_deck;
    List<Player> game_players;
}
