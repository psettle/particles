#version 330 core

struct point_light_config_type {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
    vec3 attenuation;
};

struct material_config_type {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

out vec4 color;
in vec3 normal_out;
in vec3 pos_out;

uniform sampler2D image;
uniform vec3 camera_position;
uniform point_light_config_type light_config;
uniform material_config_type material_config;

/*
	Calculate the effect of a point light on this fragment
	@param dirLight
		A description of the point light
	@param normal
		The normal from the fragment
	@param viewDirection
		The direction the camera looks from
	@return
		A sum of the ambient, diffuse and specular components of the point light with the fragment
*/
vec3 calculate_point_light_factor();

void main()
{
    vec3 base_colour = vec3( 1.0f, 1.0f, 1.0f );

    vec3 point_light_factor = calculate_point_light_factor();

    color = vec4( point_light_factor * base_colour, 1.0);
}


vec3 calculate_point_light_factor()
{
	vec3 view_direction = normalize( camera_position - pos_out );
	vec3 light_direction = normalize( light_config.position - pos_out );
	float diffuse = max( dot( normal_out, light_direction ), 0.0 );

	vec3 reflect_direction = reflect( -light_direction, normal_out ) ;

	float specular = pow( max( dot( view_direction, reflect_direction ), 0.0 ), material_config.shininess );

	vec3 ambient_factor = light_config.ambient * material_config.ambient;
	vec3 diffuse_factor = light_config.diffuse * diffuse * material_config.diffuse;
	vec3 specular_factor = light_config.specular * specular * material_config.specular;

    float distance = length( light_config.position - pos_out );
	
    float attenuation = 1.0f / ( light_config.attenuation.x + light_config.attenuation.y * distance + light_config.attenuation.z * distance * distance);

	return ( ambient_factor + diffuse_factor + specular_factor ) * attenuation;
}