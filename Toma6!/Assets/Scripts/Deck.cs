
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;


public class Deck : MonoBehaviour
{
    const int NUM_CARTAS = 104;
    
    public Deck()
    {
        deck = new List<Card>() ;
        for(int i = 1; i < NUM_CARTAS; i++)
        {
            int value = CalculateValueCard(i);
            deck.Add(new Card(i, value));
        }
        
    }
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    private int CalculateValueCard(int i)
    {
        int value = 1;
        value += (i % 10 == 5) ? 1 : 0;
        value += (i % 10 == 0) ? 2 : 0;
        value += (i % 11 == 0) ? 4 : 0;
        value += (i == 55) ? 1 : 0;
        return value;
    }
    public Card getCarta()
    {
        Card carta = deck[Random.Range(1,deck.Count)];
        while (carta.isUsed())
            carta = deck[Random.Range(1, deck.Count)];
        return carta;
    }
    List<Card> deck;

}
