
class render {
public:
	std::vector<std::unique_ptr<object>> sceneObjects;
	std::vector<light> lights;
	vec3 backGround = 0.0;
	double zDepth = 1.0;
public:
	render() {}

	/*
	void setObjects(std::vector<std::unique_ptr<object>> sceneObjects) { this->sceneObjects = sceneObjects; }
	*/

	void setLights(std::vector<light> lights) { this->lights = lights; }

	vec3 getPixelColor(vec3 defaultColor, const ray& r)
	{
		backGround = defaultColor;
		vec3 pixelColor = defaultColor;
		for (int k = 0; k < sceneObjects.size(); ++k)
		{
			vec3 tempColor;
			tempColor = ray_color(r, *static_cast<polygon*>(sceneObjects.at(k).get()));
			if (tempColor != defaultColor) pixelColor = tempColor;
			if (pixelColor == defaultColor)
			{
				pixelColor = ray_color(r, *static_cast<sphere*>(sceneObjects.at(k).get()));
			}

			//if (pixelColor != defaultColor) break;
		}
		zDepth = 1.0;
		return pixelColor;
	}

	color ray_color(const ray& r, sphere raySphere)
	{
		auto t = hit_sphere(raySphere.center, raySphere.radius, r);
		if (t > 0.0) {
			vec3 N = unit_vector(r.at(t) - raySphere.center);
			vec3 diffuse = 0.0;
			vec3 specularReflection = 0.0;
			vec3 reflection = 0.0;
			vec3 reflectDir = 0.0;
			for (int i = 0; i < lights.size(); ++i)
			{
				vec3 L = unit_vector(lights.at(i).direction);
				vec3 viewAngle = unit_vector(r.dir);
				reflectDir = unit_vector(viewAngle - (2 * N) * (dot(viewAngle, N)));

				if (!isInShadow(r.at(t), raySphere, r))
				{
					diffuse += lights.at(i).intensity * get_diffuse(N, L, raySphere.attribs.kd,
						lights.at(i).lightColor, raySphere.attribs.od);

					specularReflection += lights.at(i).intensity * get_specular(N, L, r, raySphere.attribs.ks,
						lights.at(i).lightColor, raySphere.attribs.os, raySphere.attribs.kgls);

					if (raySphere.attribs.refl != 0.0)
					{
						ray reflectRay(r.at(t) + (reflectDir * .0001), reflectDir);
						reflection = raySphere.attribs.refl * getPixelColor(backGround, reflectRay);
					}

				}
			}

			vec3 light = (1.0 - raySphere.attribs.refl) * diffuse + specularReflection + raySphere.attribs.refl * reflection;

			return clamp(light);
		}
		vec3 unit_direction = unit_vector(r.direction());
		t = 0.5 * (unit_direction.y() + 1.0);
		return color(0.2, 0.2, 0.2);//(1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
	}

	bool isInShadow(vec3 intersectPt, object currObject, const ray& r)
	{
		//get all of the lights 
		//run through all objects checking if a ray going from the point of 
		//intersection to the light source intersects any other object

		for (int i = 0; i < lights.size(); ++i)
		{
			for (int j = 0; j < sceneObjects.size(); ++j)
			{
				ray shadowRay = ray(intersectPt + (lights.at(i).direction / 100), lights.at(i).direction);
				if (static_cast<sphere*>(sceneObjects.at(j).get()))
				{
					sphere checkSphere = *static_cast<sphere*>(sceneObjects.at(j).get());
					if (hit_sphere(checkSphere.center, checkSphere.radius, shadowRay) > 0.0) return true;
				}
				if (static_cast<polygon*>(sceneObjects.at(j).get()))
				{
					polygon checkPoly = *static_cast<polygon*>(sceneObjects.at(j).get());
					if (hit_polygon(checkPoly, shadowRay) > 0.0) return true;
				}
			}
		}
		return false;
	}

	double hit_sphere(const point3& center, double radius, const ray& r) {
		vec3 oc = r.origin() - center;
		auto a = r.direction().length_squared();
		auto half_b = dot(oc, r.direction());
		auto c = oc.length_squared() - radius * radius;
		auto discriminant = half_b * half_b - a * c;

		if (discriminant < 0) {
			return -1.0;
		}
		else {
			double t = (-half_b - sqrt(discriminant)) / a;
			double depth = r.at(t)[2];
			//if (depth < zDepth && zDepth != 1.0) return -1.0;
			//zDepth = depth;
			return (-half_b - sqrt(discriminant)) / a;
		}
	}

	color ray_color(const ray& r, polygon rayPolygon)
	{
		auto t = hit_polygon(rayPolygon, r);
		if (t > 0.0) {
			vec3 N = cross((rayPolygon.p3 - rayPolygon.p2), (rayPolygon.p1 - rayPolygon.p2));
			vec3 diffuse = 0.0;
			vec3 specularReflection = 0.0;
			vec3 reflection = 0.0;
			vec3 reflectDir = 0.0;

			double d = dot(rayPolygon.p3, N) * -1.0;
			double t = (-1.0) * (dot(N, r.orig) + d) / (dot(N, r.dir));
			vec3 intersection = r.orig + (r.dir * t);

			for (int i = 0; i < lights.size(); ++i)
			{
				if (!isInShadow(intersection, rayPolygon, r))
				{
					vec3 L = unit_vector(lights.at(i).direction);
					vec3 viewAngle = unit_vector(r.dir);
					reflectDir = unit_vector(viewAngle - (2 * N) * (dot(viewAngle, N)));

					diffuse += lights.at(i).intensity * get_diffuse(N, L, rayPolygon.attribs.kd,
						lights.at(i).lightColor, rayPolygon.attribs.od);

					specularReflection += lights.at(i).intensity * get_specular(N, L, r, rayPolygon.attribs.ks,
						lights.at(i).lightColor, rayPolygon.attribs.os, rayPolygon.attribs.kgls);
					
					if (rayPolygon.attribs.refl != 0.0)
					{
						ray reflectRay(r.at(t) + (reflectDir * .0001), reflectDir);
						reflection = rayPolygon.attribs.refl * getPixelColor(backGround, reflectRay);
					}
					
				}
			}

			vec3 light = (1.0 - rayPolygon.attribs.refl) * diffuse + specularReflection + rayPolygon.attribs.refl * reflection;

			return clamp(light);
		}
		vec3 unit_direction = unit_vector(r.direction());
		t = 0.5 * (unit_direction.y() + 1.0);
		return color(0.2, 0.2, 0.2);//(1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
	}

	double hit_polygon(polygon hitPolygon, ray r)
	{
		//Find intersection point with polygon plane
		vec3 normal = cross((hitPolygon.p3 - hitPolygon.p2), (hitPolygon.p1 - hitPolygon.p2));
		double d = dot(hitPolygon.p3, normal) * -1.0;
		double t = (-1.0) * (dot(normal, r.orig) + d) / (dot(normal, r.dir));
		vec3 intersection = r.orig + (r.dir * t);

		//Check if intersection point is in polygon
		if (intersection[2] < zDepth && zDepth != 1.0) return -1.0;
		if (t > 0.0 && dot(normal, cross(hitPolygon.p3 - hitPolygon.p2, intersection - hitPolygon.p2)) > 0.0)
		{
			if (dot(normal, cross(hitPolygon.p2 - hitPolygon.p1, intersection - hitPolygon.p1)) > 0.0)
			{
				if (dot(normal, cross(hitPolygon.p1 - hitPolygon.p3, intersection - hitPolygon.p3)) > 0.0)
				{
					zDepth = intersection[2];
					return 1.0;
				}
			}
		}
		return -1.0;
	}

	vec3 get_diffuse(vec3 n, vec3 l, double kd, color lightColor, vec3 od)
	{
		double colors = dot(n, l);
		return kd * lightColor * od * color(max(0, colors), max(0, colors), max(0, colors));
	}

	vec3 get_specular(vec3 n, vec3 l, ray r, double ks, color lightColor, vec3 os, double kgls)
	{
		vec3 reflection = unit_vector(2.0 * n * (dot(n, l)) - l);
		vec3 viewpoint = unit_vector((r.dir) * (-1.0));
		double dotProduct = dot(reflection, viewpoint);
		double specular = pow(max(0, dotProduct), kgls);
		return ks * lightColor * os * color(specular, specular, specular);
	}

	vec3 clamp(const vec3 lightColor)
	{
		vec3 light;
		light[0] = min(1, lightColor[0]);
		light[1] = min(1, lightColor[1]);
		light[2] = min(1, lightColor[2]);
		return light;
	}
};
