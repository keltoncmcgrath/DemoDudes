struct node {
  char action[2];
  float final_val[2];
  float duration[2];
  float radius;
  struct node *next;
};

class linked_list {
private:

public:
  struct node *head, *tail;
  // Initialization function
  linked_list() {
    this->head = nullptr;
    this->tail = this->head;
  }
  // New node function
  void NewNode(char action1, float final1, float duration1, float radius1 = 0, char action2 = '\0', float final2 = 0, float duration2 = 0) {
    struct node *temp = new node;
    temp->action[0] = action1;
    temp->action[1] = action2;
    temp->final_val[0] = final1;
    temp->final_val[1] = final2;
    temp->duration[0] = duration1;
    temp->duration[1] = duration2;
    temp->radius = radius1;
    temp->next = head;
    head = temp;
  }
  // Delete node function
  void DelNode() {
    struct node *temp = new node;
    temp = head;
    head = head->next;
    delete temp;
  }
} LL;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  LL.NewNode('a', 100, 2);
  LL.NewNode('b', 200, 3);
  LL.NewNode('c', 300, 4);

  LL.DelNode();
  LL.DelNode();
  LL.DelNode();

  Serial.println(LL.head==nullptr);
}

void loop() {
  // put your main code here, to run repeatedly:
  // Serial.print(LL.head->action[0]);
  // Serial.print('\t');
  // Serial.print(LL.head->next->action[0]);
  // Serial.print('\t');
  // Serial.println(LL.head->next->next->action[0]);
}
