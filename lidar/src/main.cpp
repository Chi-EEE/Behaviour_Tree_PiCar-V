#include <cartographer/common/configuration_file_resolver.h>
#include <cartographer/common/make_unique.h>
#include <cartographer/mapping/map_builder.h>
#include <cartographer/mapping/2d/submap_2d.h>
#include <cartographer/sensor/timed_point_cloud_data.h>
#include "cartographer/mapping/proto/map_builder_options.pb.h"
#include "RPLidar.h"

#include <set>

// Edited code from "https://github.com/cartographer-project/cartographer_ros"
std::tuple<cartographer::mapping::proto::MapBuilderOptions, cartographer::mapping::proto::TrajectoryBuilderOptions> LoadOptions(
	const std::string& configuration_directory,
	const std::string& configuration_basename) {
	auto file_resolver =
		std::make_unique<cartographer::common::ConfigurationFileResolver>(
			std::vector<std::string>{configuration_directory});
	const std::string code =
		file_resolver->GetFileContentOrDie(configuration_basename);
	cartographer::common::LuaParameterDictionary lua_parameter_dictionary(
		code, std::move(file_resolver));

	return std::make_tuple(CreateMapBuilderOptions(&lua_parameter_dictionary),
		CreateTrajectoryBuilderOptions(&lua_parameter_dictionary));
}

cartographer::mapping::proto::MapBuilderOptions CreateMapBuilderOptions(
	::cartographer::common::LuaParameterDictionary* const
	lua_parameter_dictionary) {
	return
		::cartographer::mapping::CreateMapBuilderOptions(
			lua_parameter_dictionary->GetDictionary("map_builder").get());
}

cartographer::mapping::proto::TrajectoryBuilderOptions CreateTrajectoryBuilderOptions(
	::cartographer::common::LuaParameterDictionary* const
	lua_parameter_dictionary) {
	return
		::cartographer::mapping::CreateTrajectoryBuilderOptions(
			lua_parameter_dictionary->GetDictionary("trajectory_builder").get());
}

int main() {
	cartographer::mapping::proto::MapBuilderOptions map_builder_options;
	cartographer::mapping::proto::TrajectoryBuilderOptions trajectory_builder;
	std::tie(map_builder_options, trajectory_builder) = LoadOptions("", "");

	cartographer::mapping::MapBuilder map_builder(map_builder_options);

	std::set<cartographer::mapping::TrajectoryBuilderInterface::SensorId> sensor_ids;
	
	sensor_ids.insert(cartographer::mapping::TrajectoryBuilderInterface::SensorId{
		cartographer::mapping::TrajectoryBuilderInterface::SensorId::SensorType::LOCAL_SLAM_RESULT,
		"",
		});
	const int trajectory_id = map_builder.AddTrajectoryBuilder(sensor_ids, trajectory_builder, [](const int trajectory_id, const ::cartographer::common::Time time,
		const cartographer::transform::Rigid3d local_pose,
		::cartographer::sensor::RangeData range_data_in_local,
		const std::unique_ptr<
		const ::cartographer::mapping::TrajectoryBuilderInterface::
		InsertionResult>) {
		});
	// Initialize and start your lidar sensor here (e.g., using your_lidar_library).
	//RPLidar lidar;

	//while (true) {
	//	// Read lidar data in real-time.
	//	std::vector<double> lidar_angles;    // List of angles (in radians)
	//	std::vector<double> lidar_distances; // List of corresponding distances (in meters)

	//	lidar.readData(lidar_angles, lidar_distances);

	//	cartographer::common::Time time = cartographer::common::FromUniversal(0);
	//	Eigen::Vector3d sensor_pose(0.0, 0.0, 0.0);

	//	cartographer::sensor::TimedPointCloudData point_cloud_data(
	//		time, sensor_pose, cartographer::sensor::PointCloud{});

	//	for (size_t i = 0; i < lidar_angles.size(); ++i) {
	//		Eigen::Vector3f point(lidar_distances[i] * std::cos(lidar_angles[i]),
	//			lidar_distances[i] * std::sin(lidar_angles[i]), 0.0);
	//		point_cloud_data.point_cloud.push_back(point);
	//	}

	//	map_builder_options.AddSensorData(trajectory_id, point_cloud_data);

	//	map_builder_options.FinishTrajectory();
	//	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	//}


	return 0;
}
