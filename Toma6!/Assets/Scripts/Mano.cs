using System.Collections;
using System.Collections.Generic;
using System.Linq;
using Unity.VisualScripting;
using UnityEngine;

public class Mano : MonoBehaviour
{
    [SerializeField]
    private GameObject card;
    [SerializeField]
    private Vector3 handPositionCenterPos;
    [SerializeField]
    private GameObject cardContainer;
    const int NUM_STARTING_HAND = 10;
    public Mano()
    {
        mano = new List<Card>();
    }
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        //print hand 
        if (mano.Count>0)
        {
            for (int i = 0; i < mano.Count; i++)
            {
                GameObject newCarta =Instantiate(card, handPositionCenterPos, Quaternion.Euler(29, 0, 0));
                newCarta.transform.parent = cardContainer.transform;
            }
        }
    }
    public void setHand(List<Card> m) { mano = m; }
    public void addCard(Card c)
    {
        mano.Add(c);
    }
    List<Card> mano;
}
