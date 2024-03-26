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
  void DeleteNode() {
    struct node *temp = new node;
    temp = head;
    head = head->next;
    delete temp;
  }
  // Add new node at end of LL
  void NewTailNode(char action1, float final1, float duration1, float radius1 = 0, char action2 = '\0', float final2 = 0, float duration2 = 0) {
    // Construct node to be added
    struct node *temp = new node;
    temp->action[0] = action1;
    temp->action[1] = action2;
    temp->final_val[0] = final1;
    temp->final_val[1] = final2;
    temp->duration[0] = duration1;
    temp->duration[1] = duration2;
    temp->radius = radius1;
    temp->next = nullptr;
    // Insert as first node if head is null
    if(head == nullptr){
      temp->next = head;
      head = temp;
      return;
    }
    // Find last node
    struct node *last = head;
    last->next = head->next;
    while (last->next != nullptr) {
      last = last->next;
    }
    // Insert new node
    last->next = temp;
  }
  // Delete node at end of LL
  void DeleteNodeTail() {
    struct node *temp = head;
    // Delete current node if it's the last
    if(head->next == nullptr){
      head = head->next;
      delete temp;
      return;
    }
    // Find second to last node
    while (temp->next->next != nullptr){
      temp = temp->next;
    }
    // Delete last node and point second to last to null
    delete temp->next;
    temp->next = nullptr;
  }

} LL;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial) {
    ;
  }

  LL.NewTailNode('a', 100, 2);
  Serial.println(LL.head->action[0]);
  // LL.DeleteNodeTail();
  // Serial.println(LL.head->next->action[0]);
  // Serial.println(LL.head->next == nullptr);
  // LL.DeleteNodeTail();
  // while (LL.head != nullptr) {
  //   Serial.println(LL.head->action[0]);
  //   LL.head = LL.head->next;
  // }
}

void loop() {
  // put your main code here, to run repeatedly:
}
