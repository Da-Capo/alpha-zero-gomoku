#pragma once

#include <torch/script.h>  // One-stop header.

#include <string>
#include <memory>
#include <vector>
#include <future>
#include <utility>

#include <gomoku.h>

class NeuralNetwork {
 public:
  using return_type = std::vector<std::vector<double>>;

  NeuralNetwork(std::string model_path, bool use_gpu, unsigned int batch_size);
  ~NeuralNetwork();

  std::future<return_type> commit(Gomoku* gomoku);

 private:
  using task_type = std::pair<torch::Tensor, std::promise<return_type>>;

  void infer();  // infer by batch

  std::unique_ptr<std::thread> loop;  // infer thread
  bool running; // is running

  std::queue<task_type> tasks;  // tasks queue
  std::mutex lock;              // lock for tasks queue
  std::condition_variable cv;   // condition variable for tasks queue

  std::shared_ptr<torch::jit::script::Module> module;  // torch module
  unsigned int batch_size;                             // batch size
  bool use_gpu;                                        // use gpu
};
