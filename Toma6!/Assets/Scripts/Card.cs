using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Card : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }
    public Card(int num, int value)
    {
        this.num = num;
        this.value = value;
    }

    public int getNum() { return num; }
    public int getValue() { return value; }
    public bool isUsed() { return used; }
    int num;
    int value;
    bool used = false;
}
