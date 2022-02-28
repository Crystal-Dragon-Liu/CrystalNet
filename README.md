# CrystalNet
this lib is about to learn deep learning algorithm by following the idea of darknet. 

WARNING:This library is not available for commercial use!!! these source codes are just the understanding results of darknet.


## <font color="orange">1. Configuration ✅</font>

   <font color="lightblue">1.1</font> read the parameters written in .cfg file, and load them into struct NodeList.  
   
   
   1.1.1 read the parameters with <font color="blue">[net]</font> which is at the top.
   the function name is set to below.
   ```cpp
    void loadNetworkCommonConfig(NodeList* options, Network* net);
   ```

   1.1.2 read the rest of parameters for struct Network.  

   1.1.3 read weights of Networks by function loadWeights as shown below.
   ```cpp
    void loadWeight(Network* net, const char* cfgFileName);
   ```
  <font color="lightblue">1.2</font>  written weights of Networks into .crystal files.  

  ## <font color="orange">2. Network </font>
  
  <font color="lightblue">2.1</font>  create a Network and initialize it involving some works about allocating memory for parameters. the interface is like:
  ```cpp
   Network makeNetwork(int layerNum);
  ```  
 <font color="lightblue">2.2</font>  free all the data in Network, the interface is like :
  ```c
   void freeNetwork(Network* net);
  ```  
 <font color="lightblue">2.3</font>  create a Layer and initialize all the data in it.
  ```c
   Layer makeLayer();
  ```
  2.3.1 create Convolutional Layer, which is like:  
  ```c
   ConvolutionalLayer makeConvolutionalLayer(int batch, int h, int w, int c, int n, int size, int stride, int padding, ACTIVATION activation, int batch_normalize, ...);
  ```   
  2.3.2 create FullyConnected Layer, which is like:
  ```c
   FullyConnectedLayer makeFullyConnectedLayer(int batch, int inputs, int outputs, ACTIVATION activation, int batch_normalize);

  ```
  2.3.3 create Activation Layer  
  2.3.4 create SoftMax Layer  
  
  <font color="lightblue">2.4</font> forward, backward and update function  
  2.4.1 forward, backward and update Conv.  
  2.4.1 forward, backward and update FCN.  
  2.4.1 forward, backward and update .....

  <font color="lightblue">2.5</font> predict the result through Network.  
  2.5.1 calculate accuracy  
  2.5.2 calculate loss
  2.5.2 predict the segmentation result  
  2.5.3 predict the bounding box for object detection

  ## <font color="orange">3. Data</font>
  <font color="lightblue">3.1</font> load image data and store it into Image obj.
  
  <font color="lightblue">3.2</font> create a Matrix obj.

  <font color="lightblue">3.2</font> release Matrix obj.

  ## <font color="orange">4. Linear Algorithm with GPUs</font>



