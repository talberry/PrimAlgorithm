#include <iostream>
using namespace std;

int const VERTEXCOUNT = 5;

class Data {
    private:
        int sourceVertex;
        int destinationVertex;
        int edgeCost;

    public:
        Data (int source, int destination, int cost) {
            this->sourceVertex = source;
            this->destinationVertex = destination;
            this->edgeCost = cost;
        }

        int getSourceVertex() {
            return sourceVertex;
        }

        void setSourceVertex(int sourceVertex) {
            sourceVertex = sourceVertex;
        }

        int getDestinationVertex() {
            return destinationVertex;
        }

        void setDestinationVertex(int destinationVertex) {
            destinationVertex = destinationVertex;
        }

        int getEdgeCost() {
            return edgeCost;
        }

        void setEdgeCost(int edgeCost) {
            edgeCost = edgeCost;
        }

        int compare(Data* other) {
            if (this->edgeCost > other->edgeCost) return 1;
            else if (this->edgeCost < other->edgeCost) return -1;
            return 0;
        }

        void print() {
            cout << sourceVertex << " - " << destinationVertex << " -> " << edgeCost << endl;
        }
};

template <typename T> class Node {
    private:
        T* data;
        Node<T>* leftChild;
        Node<T>* rightChild;
        Node<T>* parent;
        int height;

    public:
        Node (T* data) {
            this->data = data;
            leftChild = nullptr;
            rightChild = nullptr;
            parent = nullptr;
            height = 0;
        }

        int compareData(T* other) {
            return data->compare(other);
        }

        void print() {
            data->print();
        }

        T* getData() {
            return data;
        }

        void setData(T* data) {
            this->data = data;
        }

        Node<T>* getLeftChild() {
            return leftChild;
        }

        void setLeftChild(Node<T>* leftChild) {
            this->leftChild = leftChild;
        }

        Node<T> *getRightChild() {
            return rightChild;
        }

        void setRightChild(Node<T>* rightChild) {
            this->rightChild = rightChild;
        }

        Node<T>* getParent() {
            return parent;
        }

        void setParent(Node<T>* parent) {
            this->parent = parent;
        }

        int getHeight() {
            return height;
        }

        void setHeight(int h) {
            height = h;
        }
};

template <typename T> class BinaryHeap {
    private:
        Node<T>* root;
        Node<T>* lastInsert;
        int numberOfElements;
        int height;

    public:
        BinaryHeap(T* data) {
            this->root = new Node<T>(data);
            numberOfElements = 1;
            height = 1;
        }

        BinaryHeap() {
            this->root = nullptr;
            numberOfElements = 0;
            height = 0;
        }

        ~BinaryHeap() {
            recursiveDelete(root);
        }

        void recursiveDelete(Node<T>* node) {
            if (node) {
                recursiveDelete(node->getLeftChild());
                recursiveDelete(node->getRightChild());
                delete node;
            }
        }

        void print() {
            print(root);
            cout << endl;
        }

        void print(Node<T>* node) {
            if (node == nullptr) return;
            node->print();
            print(node->getLeftChild());
            print(node->getRightChild());
        }

        void insertElement(T* data) {
            Node<T>* newNode = new Node<T>(data);
            if (root == nullptr) {
                root = newNode;
                lastInsert = root;
                root->setHeight(1);
                numberOfElements++;
                height = 1;
                return;
            }

            else if (lastInsert == root) {
                root->setLeftChild(newNode);
                newNode->setParent(root);
                lastInsert = newNode;
                newNode->setHeight(2);
                numberOfElements++;
                height++;

                if (newNode->compareData(root->getData()) < 0) {
                    T* temp = newNode->getData();
                    newNode->setData(root->getData());
                    root->setData(temp);
                }
            }

            else if (lastInsert->getParent()->getLeftChild() == lastInsert) {
                lastInsert->getParent()->setRightChild(newNode);
                newNode->setParent(lastInsert->getParent());
                lastInsert = newNode;
                newNode->setHeight(newNode->getParent()->getHeight() + 1);
                numberOfElements++;
                upHeap(newNode);
            }

            else if (numberOfElements == pow(2, height) - 1) {
                lastInsert = root;
                while (lastInsert->getLeftChild() != nullptr) {
                    lastInsert = lastInsert->getLeftChild();
                }
                lastInsert->setLeftChild(newNode);
                newNode->setParent(lastInsert);
                newNode->setHeight(lastInsert->getHeight() + 1);
                lastInsert = newNode;
                height++;
                numberOfElements++;
                upHeap(newNode);
            }

            else {
                preorderFindNext(root);
                lastInsert->setLeftChild(newNode);
                newNode->setParent(lastInsert);
                newNode->setHeight(lastInsert->getHeight() + 1);
                lastInsert = newNode;
                numberOfElements++;
                upHeap(newNode);
            }
        }

        void preorderFindNext(Node<T>* node) {
                if (node->getHeight() == (height - 1) && node->getLeftChild() == nullptr) {
                    lastInsert = node;
                    return;
                }

                if (node->getLeftChild() != nullptr) preorderFindNext(node->getLeftChild());
                if (node->getRightChild() != nullptr) preorderFindNext(node->getRightChild());
        }

        void upHeap(Node<T>* newNode) {
            while (newNode->compareData(newNode->getParent()->getData()) < 0 && newNode != root) {
                Node<T>* parent = newNode->getParent();
                T* temp = newNode->getData();
                newNode->setData(parent->getData());
                parent->setData(temp);

                newNode = parent;

                if (newNode == root) return;
            }
        }

        T* deleteMin() {
            if (numberOfElements == 0) {
                cout << "Heap is empty" << endl;
                return nullptr;
            }
            else if (numberOfElements == 1) {
                T* temp = root->getData();
                delete root;
                root = nullptr;
                lastInsert = nullptr;
                height = 0;
                numberOfElements--;
                return temp;
            }
            else {
                T* min = root->getData();

                root->setData(lastInsert->getData());

                Node<T>* tempLast = lastInsert;

                if (lastInsert->getParent()->getRightChild() == lastInsert) {
                    lastInsert = lastInsert->getParent()->getLeftChild();
                    lastInsert->getParent()->setRightChild(nullptr);
                }
                else {
                    lastInsert->getParent()->setLeftChild(nullptr);
                    if (numberOfElements == pow(2, height - 1)) {
                        lastInsert = root;
                        while (lastInsert->getRightChild() != nullptr) {
                            lastInsert = lastInsert->getRightChild();
                        }
                        height--;
                    }
                    else {
                        root->setParent(root);
                        backwardsPreorder(root);
                    }
                }

                delete tempLast;

                numberOfElements--;

                downHeap(root);

                return min;
            }
        }

        void backwardsPreorder(Node<T>* node) {
            if (node->getParent()->getRightChild() == node && node->getHeight() == height) {
                lastInsert = node;
                return;
            }

            if (node->getRightChild() != nullptr) backwardsPreorder(node->getRightChild());
            if (node->getLeftChild() != nullptr) backwardsPreorder(node->getLeftChild());
        }

        void downHeap(Node<T>* node) {
            while (true) {
                Node<T>* left = node->getLeftChild();
                Node<T>* right = node->getRightChild();
                Node<T>* min = nullptr;

                if (left != nullptr && right != nullptr) {
                    min = (left->compareData(right->getData()) < 0) ? left : right;
                } else if (left != nullptr) {
                    min = left;
                } else if (right != nullptr) {
                    min = right;
                }

                if (min != nullptr && min->compareData(node->getData()) < 0) {
                    T* temp = min->getData();
                    min->setData(node->getData());
                    node->setData(temp);

                    node = min;
                } else {
                    break;
                }
            }
        }

        Node<T>* getRoot() {
            return root;
        }

        void setRoot(Node<T>* root) {
            this->root = root;
        }

        int getNumberOfElements() {
            return numberOfElements;
        }

        void setNumberOfElements(int numberOfElements) {
            this->numberOfElements = numberOfElements;
        }

        int getHeight() {
            return height;
        }

        void setHeight(int height) {
            this->height = height;
        }
};

void runPrims(int G[VERTEXCOUNT][VERTEXCOUNT], BinaryHeap<Data>* binHeap) {
    bool visited[VERTEXCOUNT];

    memset(visited, false, sizeof(visited));

    int numEdges = 0;

    visited[0] = true;

    while (numEdges < (VERTEXCOUNT - 1)) {
        for (int i = 0; i < VERTEXCOUNT; ++i) {
            if (visited[i]) {
                for (int j = 0; j < VERTEXCOUNT; ++j) {
                    if (!visited[j] && G[i][j] != 0) {
                        Data* newData = new Data(i, j, G[i][j]);
                        binHeap->insertElement(newData);
                    }
                }
            }
        }

        Data* edge = binHeap->deleteMin();

        visited[edge->getDestinationVertex()] = true;

        numEdges++;

        edge->print();

        while (binHeap->getRoot() != nullptr) {
            binHeap->deleteMin();
        }
    }
}

int main() {
    int G[VERTEXCOUNT][VERTEXCOUNT] = {{0, 3, 65, 0, 0},
                                   {3, 0, 85, 20, 45},
                                   {65, 85, 0, 41, 77},
                                   {0, 20, 41, 0, 51},
                                   {0, 45, 77, 51, 0}};

    BinaryHeap<Data>* heap = new BinaryHeap<Data>();

    runPrims(G, heap);

    heap->~BinaryHeap();

    return 0;
}
