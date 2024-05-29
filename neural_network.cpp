#include "libtorch/include/torch/csrc/api/include/torch/torch.h"

class ChessNetImpl : public torch::nn::Module {
public:
    ChessNetImpl()
        : conv1(torch::nn::Conv2dOptions(12, 128, 3).padding(1)),
          conv2(torch::nn::Conv2dOptions(128, 256, 3).padding(1)),
          fc1(256 * 8 * 8, 1024),
          fc2(1024, 1) {
        register_module("conv1", conv1);
        register_module("conv2", conv2);
        register_module("fc1", fc1);
        register_module("fc2", fc2);
    }

    torch::Tensor forward(torch::Tensor x) {
        x = torch::relu(conv1->forward(x));
        x = torch::relu(conv2->forward(x));k
        x = x.view({-1, 256 * 8 * 8});
        x = torch::relu(fc1->forward(x));
        x = fc2->forward(x);
        return torch::softmax(x, /*dim=*/1);
    }

private:
    torch::nn::Conv2d conv1;
    torch::nn::Conv2d conv2;
    torch::nn::Linear fc1;
    torch::nn::Linear fc2;
};

TORCH_MODULE(ChessNet);

#endif

