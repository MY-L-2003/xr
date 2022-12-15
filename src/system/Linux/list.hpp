#include "om.h"

namespace System {
template <typename Data>
class List {
 public:
  typedef struct {
    Data data_;
    om_list_head_t node_;
  } Node;
  List() {
    INIT_LIST_HEAD(&(this->head_));
    &this->sem_handle_ = pthread_mutex_init(&this->sem_handle_, NULL);
    om_mutex_unlock(&this->sem_handle_);
  }

  bool Add(Data data) {
    om_mutex_lock(&this->sem_handle_);
    Node* node = static_cast<Node*>(malloc(sizeof(Node)));
    memcpy(&(node->data_), &data, sizeof(data));
    om_list_add(&(node->node_), &(this->head_));
    om_mutex_unlock(&this->sem_handle_);

    return true;
  }

  bool AddTail(Data data) {
    om_mutex_lock(&this->sem_handle_);
    Node* node = static_cast<Node*>(malloc(sizeof(Node)));
    memcpy(&(node->data_), &data, sizeof(data));
    om_list_add_tail(&(node->node_), (this->head_));
    om_mutex_unlock(&this->sem_handle_);

    return true;
  }

  void Foreach(bool (*fun)(Data&, void*), void* arg) {
    om_mutex_lock(&this->sem_handle_);
    om_list_head_t* pos;
    om_list_for_each(pos, &(this->head_)) {
      Node* data = om_list_entry(pos, Node, node_);
      if (!fun(data->data_, arg)) break;
    }
    om_mutex_unlock(&this->sem_handle_);
  }

  om_list_head_t head_;
  pthread_mutex_t sem_handle_;
};
}  // namespace System
