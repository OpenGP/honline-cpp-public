#pragma once
#include "tracker/ForwardDeclarations.h"
#include "tracker/Energy/Energy.h"
#include "tracker/Types.h"
#include <vector>

namespace energy{

enum PoseSpaceType { THUMB, FINGERS };

class PoseSpace : public Energy{
    Worker * worker = NULL;

public:
    struct Settings{
		bool enable_split_pca = true;
		float weight_proj = 1 * 10e2; //7 * 10e2
		float weight_mean = 0.5 * 10e1;
        int latent_size = 2;
        bool enable_joint_pca = false; ///< no thumb/fingers split
        bool debug_display_latent_space = false;
    } _settings;
    Settings* settings = &_settings;

public:
	
	void init(Worker * worker);
	void explore_pose_space(int pose_space_type);
    void track(LinearSystem &system, std::vector<float> theta);
private:
    void find_pixel_coordinates_pca(int rows, int cols, const VectorN &x, const Matrix_MxN &Limits, float &pixels_x, float &pixels_y);
    void draw_latent_positions_pca(const VectorN &x, const Matrix_MxN &Limits, string path, string window_name, std::vector<VectorN> &x_history);
    void compute_linear_approximation(int n, int m, const VectorN &y, const VectorN &x, const Matrix_MxN &P, Matrix_MxN &LHS_E1, VectorN &rhs_E1);
    void compute_objective(int n, int m, const VectorN &y, const VectorN &x, const Matrix_MxN &P, const Matrix_MxN &invSigma, Matrix_MxN &LHS_E1_y, Matrix_MxN &LHS_E1_x, VectorN &rhs_E1_y, VectorN &rhs_E1_x, Matrix_MxN &LHS_E2_y, Matrix_MxN &LHS_E2_x, VectorN &rhs_E2_y, VectorN &rhs_E2_x, Matrix_MxN &LHS_E3_y, Matrix_MxN &LHS_E3_x, VectorN &rhs_E3_y, VectorN &rhs_E3_x);
    void assemble_joint_system(int n, int m, const Matrix_MxN &LHS_y, const Matrix_MxN &LHS_x, const VectorN &rhs_y, const VectorN &rhs_x, Matrix_MxN &LHS, VectorN &rhs);
    void compose_system(int n, int m, Scalar alpha, Scalar beta, const VectorN &y, const VectorN &x, const Matrix_MxN &P, const Matrix_MxN &invSigma, Matrix_MxN &LHS, VectorN &rhs);
public:
    int m = 2;  ///< latent space dimension
    int m1 = 2; ///< latent space dimension (split/1)
    int m4 = 2; ///< latent space dimension (split/2)
   
	const int n = num_thetas_pose; // number of articulation parameters
    const int n1 = num_thetas_thumb;
    const int n4 = num_thetas_fingers;
    bool first_frame = true;
	float fingers_bending_latent_variable = 0;

    string subfolder_pca = "PoseSpace_PCA/";
    string path_pca;

    VectorN mu;

    Matrix_MxN P;
    Matrix_MxN Sigma;
    Matrix_MxN invSigma;
    Matrix_MxN Limits;

    Matrix_MxN P1;
    Matrix_MxN Sigma1;
    Matrix_MxN invSigma1;
    Matrix_MxN Limits1;

    Matrix_MxN P4;
    Matrix_MxN Sigma4;
    Matrix_MxN invSigma4;
    Matrix_MxN Limits4;

	VectorN theta_recovered;

/// @{ used to visualize path in latent space
    std::vector<VectorN> x_history;
    std::vector<VectorN> x1_history;
    std::vector<VectorN> x4_history;
/// @}
};

} ///< energy::
